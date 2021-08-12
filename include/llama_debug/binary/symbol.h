#ifndef LLAMADEBUG_SYMBOL_H
#define LLAMADEBUG_SYMBOL_H

#include <string>
#include <cstdint>

namespace llama_debug {

class symbol
{
public:
  virtual std::string lib() const = 0;
  virtual std::string name() const = 0;
  virtual uintptr_t address() const = 0;
};

}// namespace llama_debug

#endif// LLAMADEBUG_SYMBOL_H
