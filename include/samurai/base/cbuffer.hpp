#ifndef SAMURAI_CBUFFER_H
#define SAMURAI_CBUFFER_H

#include <mutex>
#include <cstring>
#include <memory>
#include <algorithm>
#include <condition_variable>
#include <chrono>

#include "samurai/base/types.hpp"

namespace Samurai {

template <class T>
class CircularBuffer {
public:
    CircularBuffer(size_t);
    ~CircularBuffer();

    size_t Capacity();
    size_t Occupancy();

    bool IsEmpty();
    bool IsFull();

    Result Get(T*, size_t);
    Result Put(T*, size_t);
    Result Reset();

    Result WaitBufferOccupancy(size_t);

private:
    std::mutex io_mtx;
    std::mutex sync_mtx;
    std::condition_variable semaphore;

    std::unique_ptr<T[]> buffer{};

    size_t head;
    size_t tail;
    size_t capacity;
    size_t occupancy;
};

} // namespace Samurai

#endif
