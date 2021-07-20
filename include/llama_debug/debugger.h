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

namespace llama_debug {

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

class ld_debugger
{
public:
  struct ld_debug_ctx;
  
  ld_debugger();
  ~ld_debugger();

  bool open(char *target);
  void close();
  int wait();

  std::vector<ld_module> get_modules();
  uintptr_t get_process_base();
  uint32_t read_virtual(uintptr_t offset, uint8_t *buffer, size_t size);
  ld_breakpoint *add_breakpoint(uintptr_t addr);

private:
  std::unique_ptr<ld_debug_ctx> m_ctx;
};

}// namespace llama_debug

#endif// LLAMADEBUG_DEBUGGER_H