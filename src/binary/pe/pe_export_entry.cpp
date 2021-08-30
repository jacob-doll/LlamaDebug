#include <iomanip>

#include "llama_debug/binary/pe/pe_export_entry.h"

namespace llama_debug {

pe_export_entry::pe_export_entry()
  : m_lib{},
    m_name{},
    m_address{ 0x0000000000000000 },
    m_export_rva{ 0x00000000 },
    m_forwarder_rva{ 0x00000000 },
    m_forwarder_name{},
    m_name_rva{ 0x00000000 },
    m_ordinal{ 0x00000000 }
{}

pe_export_entry::pe_export_entry(const std::string &lib,
  const std::string &name,
  uintptr_t address,
  raw_export_address data,
  const std::string &forwarder_name,
  uint32_t name_rva,
  uint16_t ordinal)
  : m_lib{ lib },
    m_name{ name },
    m_address{ address },
    m_export_rva{ data.export_rva },
    m_forwarder_rva{ data.forwarder_rva },
    m_forwarder_name{ forwarder_name },
    m_name_rva{ name_rva },
    m_ordinal{ ordinal }
{}

std::string &pe_export_entry::lib()
{
  return m_lib;
}

std::string &pe_export_entry::name()
{
  return m_name;
}

uintptr_t pe_export_entry::address() const
{
  return m_address;
}

uint32_t pe_export_entry::export_rva() const
{
  return m_export_rva;
}

uint32_t pe_export_entry::forwarder_rva() const
{
  return m_forwarder_rva;
}

std::string &pe_export_entry::forwarder_name()
{
  return m_forwarder_name;
}

uint32_t pe_export_entry::name_rva() const
{
  return m_name_rva;
}

uint16_t pe_export_entry::ordinal() const
{
  return m_ordinal;
}

void pe_export_entry::lib(const std::string &lib)
{
  m_lib = lib;
}

void pe_export_entry::name(const std::string &name)
{
  m_name = name;
}

void pe_export_entry::address(const uintptr_t address)
{
  m_address = address;
}

void pe_export_entry::export_rva(const uint32_t export_rva)
{
  m_export_rva = export_rva;
}

void pe_export_entry::forwarder_rva(const uint32_t forwarder_rva)
{
  m_forwarder_rva = forwarder_rva;
}

void pe_export_entry::forwarder_name(const std::string &forwarder_name)
{
  m_forwarder_name = forwarder_name;
}

void pe_export_entry::name_rva(const uint32_t name_rva)
{
  m_name_rva = name_rva;
}

void pe_export_entry::ordinal(const uint16_t ordinal)
{
  m_ordinal = ordinal;
}

std::ostream &pe_export_entry::print(std::ostream &os) const
{
  std::ios::fmtflags old_settings = os.flags();

  os << std::hex;
  os << "(EXPORT)" << m_name << "@" << m_lib << ":" << m_address;

  os.flags(old_settings);
  return os;
}

}// namespace llama_debug
