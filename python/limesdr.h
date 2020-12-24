#include <samurai.h>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

using namespace Samurai::LimeSDR;
namespace py = pybind11;

inline void init_limesdr_device(py::module &m) {
    py::class_<Device>(m, "Device")
        .def(py::init([](Device::Config config) {
            return std::unique_ptr<Device>(new Device(config));
        }))
        .def("EnableChannel", &Device::EnableChannel)
        .def("UpdateChannel", &Device::UpdateChannel)
        .def("DisableChannel", &Device::DisableChannel)
        .def("StartStream", &Device::StartStream)
        .def("StopStream", &Device::StopStream)
        .def("GetMaxNumberOfChannels", &Device::GetMaxNumberOfChannels)
        .def("GetNumberOfChannels", &Device::GetNumberOfChannels)
        .def("Config", []() {
            return Device::Config{};
        })
        .def("ReadStream", [](Channel& self, py::array_t<float> i, uint t) {
            py::buffer_info buffer = i.request(true);
            return self.ReadStream(static_cast<float*>(buffer.ptr), buffer.size / 2, t);
        });
}

inline void init_limesdr(py::module &m) {
    py::module m_sub = m.def_submodule("LimeSDR");

    init_limesdr_device(m_sub);
}
