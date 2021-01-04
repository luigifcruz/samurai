#include "samurai/cbuffer.hpp"

namespace Samurai {

template<class T>
CircularBuffer<T>::CircularBuffer(size_t capacity) : capacity(capacity) {
    this->Reset();
    this->buffer = std::unique_ptr<T[]>(new T[Capacity()]);
}

template<class T>
CircularBuffer<T>::~CircularBuffer() {
    buffer.reset();
}

template<class T>
Result CircularBuffer<T>::Get(T* buf, size_t size) {
    if (Capacity() < size) {
        return Result::ERROR;
    }

    if (IsEmpty()) {
        return Result::ERROR;
    }

    mtx.lock();

    size_t stage_a = MIN(size, Capacity() - head);
    size_t stage_b = (stage_a < size) ? size - stage_a : 0;

    std::copy_n(buffer.get() + head, stage_a, buf);
    std::copy_n(buffer.get(), stage_b, buf + stage_a);

    head = (head + size) % Capacity();
    occupancy -= size;

    mtx.unlock();

    return Result::SUCCESS;
}

template<class T>
Result CircularBuffer<T>::Put(T* buf, size_t size) {
    if (Capacity() < (size + Occupancy())) {
        return Result::ERROR;
    }

    mtx.lock();

    size_t stage_a = MIN(size, Capacity() - tail);
    size_t stage_b = (stage_a < size) ? size - stage_a : 0;

    std::copy_n(buf, stage_a, buffer.get() + tail);
    std::copy_n(buf + stage_a, stage_b, buffer.get());

    tail = (tail + size) % Capacity();
    occupancy += size;

    mtx.unlock();

    return Result::SUCCESS;
}

template<class T>
Result CircularBuffer<T>::Reset() {
    mtx.lock();

    this->head = 0;
    this->tail = 0;
    this->occupancy = 0;

    mtx.unlock();

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

} // namespace Samurai
