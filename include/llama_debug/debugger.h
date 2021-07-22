#ifndef LLAMADEBUG_DEBUGGER_H
#define LLAMADEBUG_DEBUGGER_H

#include <memory>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace llama_debug {

enum ld_debug_status {
  ERROR = 0,
  DEAD,
  STEP,
  BREAKPOINT,
  EXCEPTION,
  CREATE_PROCESS,
  EXIT_PROCESS,
  LOAD_MODULE
};

enum ld_exception_type {

};

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

struct ld_exception
{
  ld_exception_type type;
  uintptr_t address;
};

class ld_debugger
{
public:
  struct ld_debug_ctx;
  typedef std::function<void(ld_breakpoint)> breakpoint_cb;
  typedef std::function<void(ld_exception)> exception_cb;
  typedef std::function<void(const char *, uintptr_t, uint32_t)> process_create_cb;
  typedef std::function<void(uint32_t)> process_exit_cb;
  typedef std::function<void(const char *, uintptr_t, uint32_t)> module_load_cb;
  typedef std::function<void(const char *)> output_cb;

  ld_debugger();
  ~ld_debugger();

  bool open(char *target);
  void close();
  ld_debug_status wait();

  std::vector<ld_module> get_modules();
  uintptr_t get_process_base();
  uint32_t read_virtual(uintptr_t offset, uint8_t *buffer, size_t size);
  ld_breakpoint *add_breakpoint(uintptr_t addr);

  bool is_open() { return m_open; }

  void set_breakpoint_cb(const breakpoint_cb &callback);
  void set_exception_cb(const exception_cb &callback);
  void set_process_create_cb(const process_create_cb &callback);
  void set_process_exit_cb(const process_exit_cb &callback);
  void set_module_load_cb(const module_load_cb &callback);
  void set_output_cb(const output_cb &callback);

  breakpoint_cb &get_breakpooint_cb() { return m_breakpoint_cb; }
  exception_cb &get_exception_cb() { return m_exception_cb; }
  process_create_cb &get_process_create_cb() { return m_process_create_cb; }
  process_exit_cb &get_process_exit_cb() { return m_process_exit_cb; }
  module_load_cb &get_module_load_cb() { return m_module_load_cb; }
  output_cb &get_output_cb() { return m_output_cb; }


private:
  std::unique_ptr<ld_debug_ctx> m_ctx;
  bool m_open;

  breakpoint_cb m_breakpoint_cb;
  exception_cb m_exception_cb;
  process_create_cb m_process_create_cb;
  process_exit_cb m_process_exit_cb;
  module_load_cb m_module_load_cb;
  output_cb m_output_cb;
};

}// namespace llama_debug

#endif// LLAMADEBUG_DEBUGGER_H