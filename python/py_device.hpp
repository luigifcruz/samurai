#include <samurai/samurai.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

using namespace Samurai;
namespace py = pybind11;

inline void init_device_class(py::module &m) {
    py::class_<Device>(m, "Device")
        .def(py::init([](DeviceId dev_id) {
            return GetDeviceHandler(dev_id);
        }))
        .def("Enable", &Device::Enable)
        .def("EnableChannel", [](Device* self, Channel::Config config) {
            ChannelId ch = 0;
            auto response = self->EnableChannel(config, &ch);
            return std::make_tuple(ch, response);
        })
        .def("UpdateChannel", &Device::UpdateChannel)
        .def("DisableChannel", &Device::DisableChannel)
        .def("StartStream", &Device::StartStream)
        .def("StopStream", &Device::StopStream)
        .def("GetMaxNumberOfChannels", &Device::GetMaxNumberOfChannels)
        .def("GetNumberOfChannels", &Device::GetNumberOfChannels)
        .def("__enter__", [](Device& self) {
            ASSERT_SUCCESS(self.StartStream());
        })
        .def("__exit__", [](Device& self, void*, void*, void*) {
            ASSERT_SUCCESS(self.StopStream());
        })
        .def("WriteStream", [](Device& self, ChannelId id, py::array_t<std::complex<float>> i, uint t) {
            py::buffer_info buffer = i.request(true);
            return self.WriteStream(id, static_cast<float*>(buffer.ptr), buffer.size, t);
        })
        .def("WriteStream", [](Device& self, ChannelId id, py::array_t<float> i, uint t) {
            py::buffer_info buffer = i.request(true);
            return self.WriteStream(id, static_cast<float*>(buffer.ptr), buffer.size / 2, t);
        })
        .def("ReadStream", [](Device& self, ChannelId id, py::array_t<std::complex<float>> i, uint t) {
            py::buffer_info buffer = i.request(true);
            return self.ReadStream(id, static_cast<float*>(buffer.ptr), buffer.size, t);
        })
        .def("ReadStream", [](Device& self, ChannelId id, py::array_t<float> i, uint t) {
            py::buffer_info buffer = i.request(true);
            return self.ReadStream(id, static_cast<float*>(buffer.ptr), buffer.size / 2, t);
        });
}

inline void init_device_config(py::module &m) {
    py::class_<Device::Config>(m, "DeviceConfig")
        .def(py::init([]() {
            return Device::Config{};
        }))
        .def_readwrite("sampleRate", &Device::Config::sampleRate);
}

inline void init_device(py::module &m) {
    init_device_class(m);
    init_device_config(m);
}
