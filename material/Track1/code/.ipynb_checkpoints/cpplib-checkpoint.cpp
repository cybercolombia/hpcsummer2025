#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <string>
#include <cstdio>

namespace py = pybind11;



std::string hello_world() {
    std::string s = "Hello World.";
    return s;
    //printf("Hello World.\n");
}



//Pybind11 module macro that creates an entry point that will be invoked
// when the python interpreter imports an extension module.
PYBIND11_MODULE(cpplib, m) {
  m.doc() = "C++ module created with Pybind11";
  m.def("hello_world",&hello_world,"Function that prints Hello world");
}
