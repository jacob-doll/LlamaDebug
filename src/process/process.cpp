#include "llama_debug/process/process.h"
#include "llama_debug/process/platform/windows/win_process.h"

namespace llama_debug {

process::process(const std::string &name, const std::string &args)
  : m_name{ name },
    m_args{ args }
{}

std::unique_ptr<process> process::create_process(const std::string &name, const std::string &args)
{
#ifdef _WIN32
  return std::make_unique<win_process>(name, args);
#else// _WIN32
  return nullptr;
#endif
}

}// namespace llama_debug
