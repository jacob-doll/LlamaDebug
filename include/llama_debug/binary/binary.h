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
  uintptr_t base_addr() { return m_base_addr; }
  uintptr_t entry_point() { return m_entry_point; }
  // maybe main func loc?
  virtual sections_t &sections() = 0;

  virtual std::ostream &print(std::ostream &os) const;
  friend std::ostream &operator<<(std::ostream &os, const binary &binary);

protected:
  uintptr_t m_base_addr;
  uintptr_t m_entry_point;
};

};// namespace llama_debug

#endif// LLAMADEBUG_BINARY_H
