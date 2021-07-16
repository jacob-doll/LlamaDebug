#ifndef LLAMADEBUG_DEBUGGER_H
#define LLAMADEBUG_DEBUGGER_H

#include <memory>
#include <cstdint>
#include <string>
#include <vector>
#include <map>

#define LD_STATUS_ERROR 0
#define LD_STATUS_DEAD 1
#define LD_STATUS_STEP 2
#define LD_STATUS_BREAKPOINT 3
#define LD_STATUS_EXCEPTION 4
#define LD_STATUS_CREATE_PROCESS 5
#define LD_STATUS_EXIT_PROCESS 6
#define LD_STATUS_LOAD_MODULE 7

namespace llama_debug 
{

struct ld_module
{
    std::string mod_name;
    std::string file;
    uint64_t base_addr;
    uint64_t size;
};

struct ld_breakpoint
{
    uint32_t id;
    uintptr_t addr;
    uint32_t hits;
    bool enabled;
};

bool ld_debug_open(char* target);
void ld_debug_close();

int ld_debug_wait();

std::vector<ld_module> ld_get_modules();
uintptr_t ld_get_process_base();

uint32_t ld_read_virtual(uintptr_t offset, uint8_t* buffer, size_t size);

ld_breakpoint* ld_add_breakpoint(uintptr_t addr);

// class Debugger
// {
// public: 
//     static Debugger& Instance()
//     {
//         static Debugger instance;
//         return instance;
//     }

//     bool Open(char* target);
//     void Close();
    
//     int Wait();

//     std::vector<Module> GetModules();
//     uintptr_t GetProcessBase();
//     uint32_t GetProgramSize();

//     uint32_t ReadMemory(uintptr_t Offset, uint8_t* Buffer, size_t Size);

//     Breakpoint* AddBreakpoint(uintptr_t Addr);

// private:
//     std::map<uintptr_t, Breakpoint> m_Breakpoints;    

// // // Breakpoints
// //     void set_breakpoint();
// //     void remove_breakpoint();
// //     // Return a list of breakpoints
// //     void list_breakpoints();

// // // Memory Access
// //     // Get Processor State?
// //     void get_registers();
// //     // Return a list of mapped regions
// //     // ADT for each region?
// //     void get_memory_maps();

// // // Execution
// //     // attach to process that is already running
// //     void attach();
// //     // create and attach process
// //     void start();
// //     void restart();
// //     void step_into();
// //     void step_over();

// private:
//     Debugger() {}
// };

} // namespace llama_debug

#endif // LLAMADEBUG_DEBUGGER_H