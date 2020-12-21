#include <samurai.h>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

using namespace Samurai::LimeSDR;
namespace py = pybind11;

void init_limesdr_device(py::module &m) {
    py::class_<Device>(m, "Device")
        .def(py::init([]() {
            return std::unique_ptr<Device>(new Device());
        }))
        .def("EnableChannel", &Device::EnableChannel, py::return_value_policy::reference)
        .def("DisableChannel", &Device::DisableChannel)
        .def("StartStreams", &Device::StartStreams)
        .def("StopStreams", &Device::StopStreams)
        .def("SetSamplerate", &Device::SetSamplerate)
        .def("GetMaxNumberOfChannels", &Device::GetMaxNumberOfChannels)
        .def("GetNumberOfChannels", &Device::GetNumberOfChannels)
        .def("IsRunning", &Device::IsRunning);
}

void init_limesdr_channel(py::module &m) {
    py::class_<Channel>(m, "Channel")
        .def("GetId", &Channel::GetId)
        .def("GetChannel", &Channel::GetChannel)
        .def("GetMode", &Channel::GetMode)
        .def("IsRunning", &Channel::IsRunning)
        .def("SetFrequency", &Channel::SetFrequency)
        .def("GetFrequency", &Channel::GetFrequency)
        .def("SetGain", &Channel::SetGain)
        .def("GetGain", &Channel::SetGain)
        .def("SetAGC", &Channel::SetAGC)
        .def("SetCache", &Channel::SetCache)
        .def("ReadStream", [](Channel& self, py::array_t<float> i, uint t) {
            py::buffer_info buffer = i.request(true);
            return self.ReadStream(static_cast<float*>(buffer.ptr), buffer.size / 2, t);
        })
        .def("WriteStream", &Channel::WriteStream);
}

void init_limesdr(py::module &m) {
    py::module m_sub = m.def_submodule("LimeSDR");

    init_limesdr_device(m_sub);
    init_limesdr_channel(m_sub);
}
