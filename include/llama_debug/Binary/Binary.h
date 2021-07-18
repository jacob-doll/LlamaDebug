#ifndef LLAMADEBUG_BINARY_H
#define LLAMADEBUG_BINARY_H

#include <string>
#include <cstdint>

namespace llama_debug {

class binary
{
public:
  binary() = default;
  virtual ~binary();

  virtual bool from_file(const std::string &filename) = 0;
  virtual bool from_buffer(const uint8_t *buffer, uint32_t size) = 0;

  virtual void debug_print() = 0;

  virtual uintptr_t entry_point() = 0;

private:
};

};// namespace llama_debug

#endif// LLAMADEBUG_BINARY_H
