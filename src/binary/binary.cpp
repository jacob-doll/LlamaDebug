#include "llama_debug/binary/binary.h"
#include "llama_debug/binary/pe/pe_binary.h"
#include "llama_debug/binary/mmap_file.h"

namespace llama_debug {

std::ostream &binary::print(std::ostream &os) const
{
  return os;
}

std::ostream &operator<<(std::ostream &os, const binary &binary)
{
  binary.print(os);
  return os;
}

}// namespace llama_debug
