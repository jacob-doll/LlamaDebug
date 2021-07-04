#ifndef LLAMADEBUG_DEBUGGER_H
#define LLAMADEBUG_DEBUGGER_H

#include <memory>
#include <cstdint>
#include <string>
#include <vector>

#define LD_STATUS_ERROR 0
#define LD_STATUS_DEAD 1
#define LD_STATUS_STEP 2
#define LD_STATUS_BREAKPOINT 3
#define LD_STATUS_EXCEPTION 4
#define LD_STATUS_CREATE_PROCESS 5
#define LD_STATUS_EXIT_PROCESS 6
#define LD_STATUS_LOAD_MODULE 7

namespace LlamaDebug 
{

struct Module
{
    std::string modName;
    std::string file;
    uint64_t baseAddr;
    uint64_t size;
};

class Debugger
{
public:
    typedef void (*OutputCallback)(const char*);
    
    Debugger();

    bool Open(char* target);
    void Close();
    
    int Wait();

    std::vector<Module> GetModules();

    void SetOutputCallback(OutputCallback output_cb) { m_outputCallback = output_cb; }
    OutputCallback &GetOutputCallback() { return m_outputCallback; }



// // Breakpoints
//     void set_breakpoint();
//     void remove_breakpoint();
//     // Return a list of breakpoints
//     void list_breakpoints();

// // Memory Access
//     // Get Processor State?
//     void get_registers();
//     // Return a list of mapped regions
//     // ADT for each region?
//     void get_memory_maps();

// // Execution
//     // attach to process that is already running
//     void attach();
//     // create and attach process
//     void start();
//     void restart();
//     void step_into();
//     void step_over();

private:
    class Impl;
    Impl* p_impl;

    OutputCallback m_outputCallback;
};

} // namespace LlamaDebug

#endif // LLAMADEBUG_DEBUGGER_H