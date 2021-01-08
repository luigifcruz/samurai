#include <samurai/samurai.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

using namespace Samurai;
namespace py = pybind11;

template<class T>
void init_circular_buffer(py::module &m) {
    using Class = CircularBuffer<T>;
    py::class_<Class>(m, "CircularBuffer")
        .def(py::init([](size_t size) {
            return std::make_unique<Class>(size);
        }))
        .def("Put", [](CircularBuffer<float>& self, py::array_t<T> i) {
            py::buffer_info buffer = i.request(true);
            return self.Put(static_cast<T*>(buffer.ptr), buffer.size);
        })
        .def("Get", [](CircularBuffer<float>& self, py::array_t<T> i) {
            py::buffer_info buffer = i.request(true);
            return self.Get(static_cast<T*>(buffer.ptr), buffer.size);
        });
}
