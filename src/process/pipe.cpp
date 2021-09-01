#include "llama_debug/process/pipe.h"

#include "llama_debug/process/platform/windows/win_pipe.h"

namespace llama_debug {

std::unique_ptr<pipe> pipe::create_pipe()
{
#ifdef _WIN32
  return std::make_unique<win_pipe>();
#else// _WIN32
  return nullptr;
#endif
}

}// namespace llama_debug
