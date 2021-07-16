#include <pybind11/pybind11.h>
#include <LlamaDebug/Debugger.h>

// Python Bindings
namespace py = pybind11;
using namespace LlamaDebug;

PYBIND11_MODULE(LlamaDebug, m) {
    m.attr("LD_STATUS_ERROR") = LD_STATUS_ERROR;
    m.attr("LD_STATUS_DEAD") = LD_STATUS_DEAD;
    m.attr("LD_STATUS_STEP") = LD_STATUS_STEP;
    m.attr("LD_STATUS_BREAKPOINT") = LD_STATUS_BREAKPOINT;
    m.attr("LD_STATUS_EXCEPTION") = LD_STATUS_EXCEPTION;
    m.attr("LD_STATUS_CREATE_PROCESS") = LD_STATUS_CREATE_PROCESS;
    m.attr("LD_STATUS_EXIT_PROCESS") = LD_STATUS_EXIT_PROCESS;
    m.attr("LD_STATUS_LOAD_MODULE") = LD_STATUS_LOAD_MODULE;

    py::class_<Module>(m, "Module")
        .def_readwrite("ModName", &Module::ModName)
        .def_readwrite("File", &Module::File)
        .def_readwrite("BaseAddr", &Module::BaseAddr)
        .def_readwrite("Size", &Module::Size);

    py::class_<Breakpoint>(m, "Breakpoint")
        .def_readwrite("ID", &Breakpoint::ID)
        .def_readwrite("Addr", &Breakpoint::Addr)
        .def_readwrite("Hits", &Breakpoint::Hits)
        .def_readwrite("Enabled", &Breakpoint::Enabled);

    py::class_<Debugger>(m, "Debugger")
        .def_static("Instance", &Debugger::Instance)
        .def("Open", &Debugger::Open)
        .def("Close", &Debugger::Close)
        .def("Wait", &Debugger::Wait)
        .def("GetModules", &Debugger::GetModules)
        .def("GetProcessBase", &Debugger::GetProcessBase)
        .def("GetProgramSize", &Debugger::GetProgramSize)
        .def("ReadMemory", &Debugger::ReadMemory);
}