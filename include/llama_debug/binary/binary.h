#ifndef LLAMADEBUG_BINARY_H
#define LLAMADEBUG_BINARY_H

#include <cstdint>
#include <ostream>

#include "llama_debug/types.h"
#include "llama_debug/binary/section.h"
#include "llama_debug/binary/symbol.h"

namespace llama_debug {

class binary
{
public:
  binary();
  binary(const std::string &name);

  std::string &name();
  uintptr_t base_addr();
  uintptr_t entry_point();
  sections_t &sections();
  symbols_t &symbols();

  virtual std::ostream &print(std::ostream &os) const;
  friend std::ostream &operator<<(std::ostream &os, const binary &binary);

protected:
  std::string m_name;
  uintptr_t m_base_addr;
  uintptr_t m_entry_point;
  sections_t m_sections;
  symbols_t m_symbols;
};

};// namespace llama_debug

#endif// LLAMADEBUG_BINARY_H
