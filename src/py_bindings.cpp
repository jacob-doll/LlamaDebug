#include <pybind11/pybind11.h>
#include <llama_debug/debugger.h>

// Python Bindings
namespace py = pybind11;
using namespace llama_debug;

PYBIND11_MODULE(llama_debug, m)
{
  m.attr("STATUS_ERROR") = LD_STATUS_ERROR;
  m.attr("STATUS_DEAD") = LD_STATUS_DEAD;
  m.attr("STATUS_STEP") = LD_STATUS_STEP;
  m.attr("STATUS_BREAKPOINT") = LD_STATUS_BREAKPOINT;
  m.attr("STATUS_EXCEPTION") = LD_STATUS_EXCEPTION;
  m.attr("STATUS_CREATE_PROCESS") = LD_STATUS_CREATE_PROCESS;
  m.attr("STATUS_EXIT_PROCESS") = LD_STATUS_EXIT_PROCESS;
  m.attr("STATUS_LOAD_MODULE") = LD_STATUS_LOAD_MODULE;

  py::class_<ld_module>(m, "module")
    .def_readwrite("mod_name", &ld_module::mod_name)
    .def_readwrite("file", &ld_module::file)
    .def_readwrite("base_addr", &ld_module::base_addr)
    .def_readwrite("size", &ld_module::size);

  py::class_<ld_breakpoint>(m, "breakpoint")
    .def_readwrite("id", &ld_breakpoint::id)
    .def_readwrite("addr", &ld_breakpoint::addr)
    .def_readwrite("hits", &ld_breakpoint::hits)
    .def_readwrite("enabled", &ld_breakpoint::enabled);

  m.def("debug_open", &ld_debug_open);
  m.def("debug_close", &ld_debug_close);
  m.def("debug_wait", &ld_debug_wait);
  m.def("get_modules", &ld_get_modules);
  m.def("get_process_base", &ld_get_process_base);
  m.def("read_virtual", &ld_read_virtual);
  m.def("add_breakpoint", &ld_add_breakpoint);
}