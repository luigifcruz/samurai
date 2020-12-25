#include <samurai/samurai.h>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

using namespace Samurai;
namespace py = pybind11;

inline void init_channel_config(py::module &m) {
    py::class_<Channel::Config>(m, "ChannelConfig")
        .def(py::init([]() {
            return Channel::Config{};
        }))
        .def_readwrite("mode", &Channel::Config::mode)
        .def_readwrite("dataFmt", &Channel::Config::dataFmt)
        .def_readwrite("calibrate", &Channel::Config::calibrate);
}

inline void init_channel_state(py::module &m) {
    py::class_<Channel::State>(m, "ChannelState")
        .def(py::init([]() {
            return Channel::State{};
        }))
        .def_readwrite("frequency", &Channel::State::frequency)
        .def_readwrite("manualGain", &Channel::State::manualGain)
        .def_readwrite("enableAGC", &Channel::State::enableAGC);
}

 inline void init_channel(py::module &m) {
    init_channel_config(m);
    init_channel_state(m);
}
