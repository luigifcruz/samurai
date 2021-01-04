#include <samurai/cbuffer.hpp>

#include <iostream>

using namespace Samurai;

int main() {
    // Basic Test
    float a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    float b[8] = {};

    CircularBuffer<float> buffer(8);
    assert(buffer.Capacity() == 8);
    assert(buffer.Occupancy() == 0);
    assert(buffer.IsFull() == false);
    assert(buffer.IsEmpty() == true);

    ASSERT_SUCCESS(buffer.Put((float*)&a, 8));
    assert(buffer.Occupancy() == 8);
    assert(buffer.IsFull() == true);

    ASSERT_SUCCESS(buffer.Get((float*)&b, 8));
    assert(buffer.Occupancy() == 0);
    for (int i = 0; i < 8; i++)
        assert(b[i] == a[i]);

    // Test Reset
    ASSERT_SUCCESS(buffer.Reset());
    assert(buffer.Occupancy() == 0);
    assert(buffer.IsEmpty() == true);

    // Continuity Test
    float c[] = {1, 2, 3, 4, 5, 6, 7, 8};
    float d[] = {9, 8, 7, 6};
    float e[4] = {};
    float f[8] = {};

    CircularBuffer<float> buffer2(8);

    ASSERT_SUCCESS(buffer2.Put((float*)&d, 4));
    assert(buffer2.Occupancy() == 4);

    ASSERT_SUCCESS(buffer2.Get((float*)&e, 4));
    assert(buffer2.Occupancy() == 0);
    for (int i = 0; i < 4; i++)
        assert(d[i] == e[i]);

    ASSERT_SUCCESS(buffer2.Put((float*)&c, 8));
    assert(buffer2.Occupancy() == 8);

    ASSERT_SUCCESS(buffer2.Get((float*)&f, 8));
    assert(buffer2.Occupancy() == 0);
    for (int i = 0; i < 8; i++)
        assert(c[i] == f[i]);

    return 0;
}

