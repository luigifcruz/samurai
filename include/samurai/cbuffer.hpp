#ifndef SAMURAI_CBUFFER_H
#define SAMURAI_CBUFFER_H

#include "samurai/types.hpp"

#include <mutex>
#include <cstring>
#include <memory>
#include <algorithm>

namespace Samurai {

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

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

private:
    std::mutex mtx;
    std::unique_ptr<T[]> buffer;

    size_t head;
    size_t tail;
    size_t capacity;
    size_t occupancy;
};

template class CircularBuffer<char>;
template class CircularBuffer<float>;

} // namespace Samurai

#endif
