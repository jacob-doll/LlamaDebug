#ifndef LLAMADEBUG_PROCESS_H
#define LLAMADEBUG_PROCESS_H

#include <memory>
#include <string>
#include <vector>

#include "llama_debug/process/memory.h"
#include "llama_debug/process/types.h"
#include "llama_debug/process/pipe.h"

namespace llama_debug {

class binary;

class process
{
public:
  static std::unique_ptr<process> create_process(
    const std::string &name,
    const std::string &args);

  virtual void close() = 0;
  virtual void kill(uint32_t exit_code) = 0;
  virtual bool is_active() = 0;
  virtual mapped_regions_t mapped_regions() = 0;
  virtual size_t read_memory(
    const uintptr_t addr,
    uint8_t *buffer,
    size_t size) = 0;

  virtual std::unique_ptr<binary> carve_binary(std::string &name) = 0;

  template<typename T>
  size_t read_memory(const uintptr_t addr, T &value)
  {
    return read_memory(addr, (uint8_t *)(&value), sizeof(value));
  }

  pipe &std_out();
  pipe &std_in();

  uintptr_t base_addr();
  std::string &name();

protected:
  process(const std::string &name, const std::string &args);

protected:
  std::string m_name;
  std::string m_args;
  ldpid_t m_pid;
  std::unique_ptr<pipe> m_std_out;
  std::unique_ptr<pipe> m_std_in;
};

}// namespace llama_debug

#endif// LLAMADEBUG_PROCESS_H
