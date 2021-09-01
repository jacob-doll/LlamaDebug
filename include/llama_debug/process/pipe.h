#ifndef LLAMADEBUG_PIPE_H
#define LLAMADEBUG_PIPE_H

#include <cstdint>
#include <vector>
#include <memory>

namespace llama_debug {

class pipe
{
public:
  static std::unique_ptr<pipe> create_pipe();

  virtual const std::vector<uint8_t> read() = 0;
  virtual void write(const std::vector<uint8_t> &data) = 0;
  virtual void close() = 0;
};

}// namespace llama_debug


#endif// LLAMADEBUG_PIPE_H
