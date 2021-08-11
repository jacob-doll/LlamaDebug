#ifndef LLAMADEBUG_BINARY_H
#define LLAMADEBUG_BINARY_H

#include <string>
#include <vector>
#include <cstdint>

#include <llama_debug/types.h>
#include <llama_debug/binary/section.h>

namespace llama_debug {

// add more abstraction

struct symbol
{
  std::string lib;
  std::string name;
  uintptr_t address;
};

class binary
{
public:
  static binary *from_file(const std::string &filename);
  static binary *from_buffer(const uint8_t *buffer, uint32_t size);

  uintptr_t base_addr() { return m_base_addr; }
  uintptr_t entry_point() { return m_entry_point; }
  // maybe main func loc?
  virtual sections_t &sections() = 0;
  virtual std::vector<symbol> &symbols() = 0;

protected:
  uintptr_t m_base_addr;
  uintptr_t m_entry_point;
};

};// namespace llama_debug

#endif// LLAMADEBUG_BINARY_H
