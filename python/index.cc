#include "limesdr.h"

#include <samurai.h>

#include <pybind11/pybind11.h>

using namespace Samurai;

PYBIND11_MODULE(samurai, m) {
    py::enum_<Mode>(m, "Mode")
        .value("NONE", MODE_NONE)
        .value("RX", MODE_RX)
        .value("TX", MODE_RX)
        .export_values();

    init_limesdr(m);
}
