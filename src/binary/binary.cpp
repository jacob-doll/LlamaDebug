#include "llama_debug/binary/binary.h"
#include "llama_debug/binary/pe/pe_binary.h"
#include "llama_debug/binary/mmap_file.h"

namespace llama_debug {

binary::binary() = default;

binary::binary(const std::string &name)
  : m_name{ name }
{}

std::string &binary::name()
{
  return m_name;
}

uintptr_t binary::base_addr()
{
  return m_base_addr;
}

uintptr_t binary::entry_point()
{
  return m_entry_point;
}

sections_t &binary::sections()
{
  return m_sections;
}

symbols_t &binary::symbols()
{
  return m_symbols;
}

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
