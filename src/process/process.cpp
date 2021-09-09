#include "llama_debug/process/process.h"
#include "llama_debug/process/platform/windows/win_process.h"

namespace llama_debug {

process::process(const std::string &name, const std::string &args)
  : m_name{ name },
    m_args{ args }
{
  m_std_out = pipe::create_pipe();
  m_std_in = pipe::create_pipe();
}

pipe &process::std_out()
{
  return *m_std_out;
}

pipe &process::std_in()
{
  return *m_std_in;
}

uintptr_t process::base_addr()
{
  for (mapped_region reg : this->mapped_regions()) {
    if (reg.mapped_file_name.find(this->m_name) != std::string::npos) {
      return reg.base_addr;
    }
  }
  return 0;
}

std::unique_ptr<process> process::create_process(const std::string &name, const std::string &args)
{
#ifdef _WIN32
  return std::make_unique<win_process>(name, args);
#else// _WIN32
  return nullptr;
#endif
}

}// namespace llama_debug
