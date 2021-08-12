#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

#include "llama_debug/debugger.h"

// Python Bindings
namespace py = pybind11;
using namespace llama_debug;

PYBIND11_MODULE(llama_debug, m)
{
  py::enum_<ld_debug_status>(m, "debug_status")
    .value("ERROR", ld_debug_status::ERROR)
    .value("DEAD", ld_debug_status::DEAD)
    .value("STEP", ld_debug_status::STEP)
    .value("BREAKPOINT", ld_debug_status::BREAKPOINT)
    .value("EXCEPTION", ld_debug_status::EXCEPTION)
    .value("CREATE_PROCESS", ld_debug_status::CREATE_PROCESS)
    .value("EXIT_PROCESS", ld_debug_status::EXIT_PROCESS)
    .value("LOAD_MODULE", ld_debug_status::LOAD_MODULE)
    .export_values();

  py::enum_<ld_exception_type>(m, "exception_type")
    .export_values();

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

  py::class_<ld_exception>(m, "exception")
    .def_readwrite("type", &ld_exception::type)
    .def_readwrite("address", &ld_exception::address);

  py::class_<ld_debugger>(m, "debugger")
    .def(py::init<>())
    .def("open", &ld_debugger::open)
    .def("close", &ld_debugger::close)
    .def("wait", &ld_debugger::wait)
    .def("get_modules", &ld_debugger::get_modules)
    .def("get_process_base", &ld_debugger::get_process_base)
    .def("read_virtual", &ld_debugger::read_virtual)
    .def("is_open", &ld_debugger::is_open)
    .def("add_breakpoint", &ld_debugger::add_breakpoint)
    .def("set_breakpoint_cb", &ld_debugger::set_breakpoint_cb)
    .def("set_exception_cb", &ld_debugger::set_exception_cb)
    .def("set_process_create_cb", &ld_debugger::set_process_create_cb)
    .def("set_process_exit_cb", &ld_debugger::set_process_exit_cb)
    .def("set_module_load_cb", &ld_debugger::set_module_load_cb)
    .def("set_output_cb", &ld_debugger::set_output_cb);
}