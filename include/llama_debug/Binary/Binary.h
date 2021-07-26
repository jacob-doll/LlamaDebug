#ifndef LLAMADEBUG_BINARY_H
#define LLAMADEBUG_BINARY_H

#include <string>
#include <vector>
#include <cstdint>

namespace llama_debug {

struct section
{
  std::string name;
  uint32_t phys_size;
  uint32_t virt_size;
  uintptr_t phys_addr;
  uintptr_t virt_addr;
  uint32_t flags;
};

struct symbol
{
  std::string lib;
  std::string name;
  uintptr_t address;
};

class binary
{
public:
  binary() = default;

  static binary* from_file(const std::string &filename);
  static binary* from_buffer(const uint8_t *buffer, uint32_t size);

  virtual void debug_print() = 0;

  uintptr_t base_addr() { return m_base_addr; }
  uintptr_t entry_point() { return m_entry_point; }
  // maybe main func loc?
  std::vector<section> &sections() { return m_sections; }
  std::vector<symbol> &symbols() { return m_symbols; }

protected:
  uintptr_t m_base_addr;
  uintptr_t m_entry_point;
  std::vector<section> m_sections;
  std::vector<symbol> m_symbols;
};

};// namespace llama_debug

#endif// LLAMADEBUG_BINARY_H
