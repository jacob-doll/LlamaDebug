#include "llama_debug/binary/symbol.h"

namespace llama_debug {

std::ostream &symbol::print(std::ostream &os) const
{
  return os;
}

std::ostream &operator<<(std::ostream &os, const symbol &symbol)
{
  symbol.print(os);
  return os;
}

}// namespace llama_debug
