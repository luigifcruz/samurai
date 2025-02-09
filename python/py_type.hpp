#include <samurai/samurai.hpp>

#include <pybind11/pybind11.h>

using namespace Samurai;
namespace py = pybind11;

inline void init_types_mode(py::module &m) {
    py::enum_<Mode>(m, "Mode")
        .value("NONE", Mode::NONE)
        .value("RX", Mode::RX)
        .value("TX", Mode::TX);
}

inline void init_types_result(py::module &m) {
    py::enum_<Result>(m, "Result")
        .value("SUCCESS", Result::SUCCESS)
        .value("ERROR", Result::ERROR)
        .value("ERROR_DEVICE_API", Result::ERROR_DEVICE_API)
        .value("ERROR_INVALID_DATA_TYPE", Result::ERROR_INVALID_DATA_TYPE)
        .value("ERROR_FAILED_TO_FETCH_DATA", Result::ERROR_FAILED_TO_FETCH_DATA)
        .value("ERROR_FAILED_TO_CONFIGURE_DEVICE", Result::ERROR_FAILED_TO_CONFIGURE_DEVICE)
        .value("ERROR_FAILED_TO_INIT_DEVICE", Result::ERROR_FAILED_TO_INIT_DEVICE)
        .value("ERROR_FAILED_TO_OPEN_DEVICE", Result::ERROR_FAILED_TO_OPEN_DEVICE)
        .value("ERROR_FAILED_TO_FIND_CHANNEL", Result::ERROR_FAILED_TO_FIND_CHANNEL)
        .value("ERROR_FAILED_TO_FIND_DEVICE", Result::ERROR_FAILED_TO_FIND_DEVICE)
        .value("ERROR_MAX_NUMBER_OF_CHANNELS_REACHED", Result::ERROR_MAX_NUMBER_OF_CHANNELS_REACHED)
        .value("ERROR_CHANNEL_NOT_READY", Result::ERROR_CHANNEL_NOT_READY)
        .value("ERROR_DEVICE_NOT_READY", Result::ERROR_DEVICE_NOT_READY)
        .value("ERROR_CANT_ALLOCATE_MEMORY", Result::ERROR_CANT_ALLOCATE_MEMORY)
        .value("ERROR_FORMAT_NOT_SUPPORTED", Result::ERROR_FORMAT_NOT_SUPPORTED);
}

inline void init_types_format(py::module &m) {
    py::enum_<Format>(m, "Format")
        .value("F32", Format::F32)
        .value("I16", Format::I16)
        .value("I12", Format::I12);
}

inline void init_types_deviceid(py::module &m) {
    py::enum_<DeviceId>(m, "DeviceId")
        #ifdef SAMURAI_LIMESDR_ENABLED
        .value("LimeSDR", DeviceId::LimeSDR)
        #endif
        #ifdef SAMURAI_AIRSPY_ENABLED
        .value("Airspy", DeviceId::Airspy)
        #endif
        ;
}

inline void init_types(py::module &m) {
    init_types_mode(m);
    init_types_result(m);
    init_types_format(m);
    init_types_deviceid(m);
}
