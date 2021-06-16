#include "py_channel.hpp"
#include "py_device.hpp"
#include "py_type.hpp"
#include "py_cbuffer.hpp"

#include <pybind11/pybind11.h>

using namespace Samurai;

PYBIND11_MODULE(samurai, m) {
    m.def("SAMURAI_CHECK_THROW", [](Result result) {
        SAMURAI_CHECK_THROW(result);
    });

    init_types(m);
    init_device(m);
    init_channel(m);
    init_circular_buffer<float>(m);
}
