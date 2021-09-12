#include <iomanip>

#include "llama_debug/binary/pe/pe_import_entry.h"

namespace llama_debug {

pe_import_entry::pe_import_entry()
  : m_lib{},
    m_name{},
    m_address{ 0x0000000000000000 },
    m_hint{ 0x0000 },
    m_name_rva{ 0x0000000000000000 },
    m_address_rva{ 0x0000000000000000 }
{}

pe_import_entry::pe_import_entry(const std::string &lib,
  const raw_hint_name *data,
  uintptr_t address,
  uint64_t name_rva,
  uint64_t address_rva)
  : m_lib{ lib },
    m_name{ data->name },
    m_address{ address },
    m_hint{ data->hint },
    m_name_rva{ name_rva },
    m_address_rva{ address_rva }
{}

pe_import_entry::pe_import_entry(const std::string &lib,
  uint16_t hint,
  const std::string &name,
  uintptr_t address,
  uint64_t name_rva,
  uint64_t address_rva)
  : m_lib{ lib },
    m_name{ name },
    m_address{ address },
    m_hint{ hint },
    m_name_rva{ name_rva },
    m_address_rva{ address_rva }
{}

std::string &pe_import_entry::lib()
{
  return m_lib;
}

std::string &pe_import_entry::name()
{
  return m_name;
}

uintptr_t pe_import_entry::address() const
{
  return m_address;
}

uint16_t pe_import_entry::hint() const
{
  return m_hint;
}

uint64_t pe_import_entry::name_rva() const
{
  return m_name_rva;
}

uint64_t pe_import_entry::address_rva() const
{
  return m_address_rva;
}

void pe_import_entry::lib(const std::string &lib)
{
  m_lib = lib;
}

void pe_import_entry::name(const std::string &name)
{
  m_name = name;
}

void pe_import_entry::address(const uintptr_t address)
{
  m_address = address;
}

void pe_import_entry::hint(const uint16_t hint)
{
  m_hint = hint;
}

void pe_import_entry::name_rva(const uint64_t name_rva)
{
  m_name_rva = name_rva;
}

void pe_import_entry::address_rva(const uint64_t address_rva)
{
  m_address_rva = address_rva;
}

std::ostream &pe_import_entry::print(std::ostream &os) const
{
  std::ios::fmtflags old_settings = os.flags();

  os << std::hex;
  os << "(IMPORT)" << m_name << "@" << m_lib << ":" << m_address;

  os.flags(old_settings);
  return os;
}

}// namespace llama_debug
