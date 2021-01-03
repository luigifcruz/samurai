#include "py_channel.h"
#include "py_device.h"
#include "py_types.h"

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
