#ifndef LLAMADEBUG_SYMBOL_H
#define LLAMADEBUG_SYMBOL_H

#include <string>
#include <cstdint>

namespace llama_debug {

class symbol
{
public:
  virtual std::string &lib() = 0;
  virtual std::string &name() = 0;
  virtual uintptr_t address() const = 0;

  virtual std::ostream &print(std::ostream &os) const;
  friend std::ostream &operator<<(std::ostream &os, const symbol &symbol);
};

}// namespace llama_debug

#endif// LLAMADEBUG_SYMBOL_H
