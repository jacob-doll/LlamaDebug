#ifndef __LAMADEBUG__DEBUGGER_H__
#define __LAMADEBUG__DEBUGGER_H__

#include <memory>

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

class Debugger
{
public:
    typedef void (*OutputCallback)(const char*);
    
    Debugger();

    bool open(char* target);
    void close();

    void setOutputCallback(OutputCallback output_cb) { m_outputCallback = output_cb; }
    OutputCallback &getOutputCallback() { return m_outputCallback; }

    int wait();

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

#endif // __LAMADEBUG__DEBUGGER_H__