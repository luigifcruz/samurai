#include "samurai/base/cbuffer.hpp"

using namespace std::chrono_literals;

namespace Samurai {

template<class T>
CircularBuffer<T>::CircularBuffer(size_t capacity) : capacity(capacity) {
    this->Reset();
    this->buffer = std::unique_ptr<T[]>(new T[Capacity()]);
}

template<class T>
CircularBuffer<T>::~CircularBuffer() {
    semaphore.notify_all();
    io_mtx.lock();
    buffer.reset();
}

template<class T>
Result CircularBuffer<T>::WaitBufferOccupancy(size_t size) {
    std::unique_lock<std::mutex> sync(sync_mtx);
    while (Occupancy() < size) {
        if (semaphore.wait_for(sync, 5s) == std::cv_status::timeout)
            return Result::ERROR_TIMEOUT;
    }
    return Result::SUCCESS;
}

template<class T>
Result CircularBuffer<T>::Get(T* buf, size_t size) {
    if (Capacity() < size) {
        return Result::ERROR_BEYOND_CAPACITY;
    }

    Result res = WaitBufferOccupancy(size);
    if (res != Result::SUCCESS)
        goto exception;

    {
        const std::lock_guard<std::mutex> lock(io_mtx);

        size_t stage_a = MIN(size, Capacity() - head);
        size_t stage_b = (stage_a < size) ? size - stage_a : 0;

        std::copy_n(buffer.get() + head, stage_a, buf);
        std::copy_n(buffer.get(), stage_b, buf + stage_a);

        head = (head + size) % Capacity();
        occupancy -= size;
    }

exception:
    return res;
}

template<class T>
Result CircularBuffer<T>::Put(T* buf, size_t size) {
    if (Capacity() < size) {
        return Result::ERROR_BEYOND_CAPACITY;
    }

    {
        const std::lock_guard<std::mutex> lock(io_mtx);

        if (Capacity() < (Occupancy() + size)) {
            printf("o");
            occupancy = 0;
            head = tail;
        }

        size_t stage_a = MIN(size, Capacity() - tail);
        size_t stage_b = (stage_a < size) ? size - stage_a : 0;

        std::copy_n(buf, stage_a, buffer.get() + tail);
        std::copy_n(buf + stage_a, stage_b, buffer.get());

        tail = (tail + size) % Capacity();
        occupancy += size;
    }

    semaphore.notify_all();
    return Result::SUCCESS;
}

template<class T>
Result CircularBuffer<T>::Reset() {
    {
        const std::lock_guard<std::mutex> lock(io_mtx);
        this->head = 0;
        this->tail = 0;
        this->occupancy = 0;
    }

    semaphore.notify_all();
    return Result::SUCCESS;
}

template<class T>
size_t CircularBuffer<T>::Capacity() {
    return this->capacity;
}

template<class T>
size_t CircularBuffer<T>::Occupancy() {
    return this->occupancy;
}

template<class T>
bool CircularBuffer<T>::IsEmpty() {
    return Occupancy() == 0;
}

template<class T>
bool CircularBuffer<T>::IsFull() {
    return Occupancy() == Capacity();
}

template class CircularBuffer<char>;
template class CircularBuffer<float>;
template class CircularBuffer<std::complex<float>>;

} // namespace Samurai
