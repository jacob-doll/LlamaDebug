#include "llama_debug/binary/section.h"

namespace llama_debug {

std::ostream &section::print(std::ostream &os) const
{
  return os;
}

std::ostream &operator<<(std::ostream &os, const section &section)
{
  section.print(os);
  return os;
}

}// namespace llama_debug
