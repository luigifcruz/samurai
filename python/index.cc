#include "py_channel.hpp"
#include "py_device.hpp"
#include "py_types.hpp"

#include <pybind11/pybind11.h>

using namespace Samurai;

PYBIND11_MODULE(samurai, m) {
    m.def("ASSERT_SUCCESS", [](Result result) {
        ASSERT_SUCCESS(result);
    });

    init_types(m);
    init_device(m);
    init_channel(m);
}
