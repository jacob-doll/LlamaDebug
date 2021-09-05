#ifndef LLAMADEBUG_PROCESS_H
#define LLAMADEBUG_PROCESS_H

#include <memory>
#include <string>

namespace llama_debug {

class process
{
public:
  static std::unique_ptr<process> create_process(
    const std::string &name,
    const std::string &args);

protected:
  process(const std::string &name, const std::string &args);

protected:
  std::string m_name;
  std::string m_args;
};

}// namespace llama_debug

#endif// LLAMADEBUG_PROCESS_H
