#ifndef LLAMADEBUG_PROCESS_H
#define LLAMADEBUG_PROCESS_H

#include <memory>
#include <string>
#include <vector>

#include "llama_debug/process/types.h"
#include "llama_debug/process/pipe.h"

namespace llama_debug {

class process
{
public:
  static std::unique_ptr<process> create_process(
    const std::string &name,
    const std::string &args);

  virtual void close() = 0;
  virtual void kill(uint32_t exit_code) = 0;
  virtual bool is_active() = 0;

  pipe &std_out();
  pipe &std_in();

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
