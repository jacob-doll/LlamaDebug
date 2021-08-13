#include "llama_debug/binary/pe/import_entry.h"

namespace llama_debug {

import_entry::import_entry()
  : m_lib{},
    m_name{},
    m_address{ 0x0000000000000000 },
    m_hint{ 0x0000 },
    m_name_rva{ 0x0000000000000000 },
    m_address_rva{ 0x0000000000000000 }
{}

import_entry::import_entry(const std::string &lib,
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

std::string &import_entry::lib()
{
  return m_lib;
}

std::string &import_entry::name()
{
  return m_name;
}

uintptr_t import_entry::address() const
{
  return m_address;
}

uint16_t import_entry::hint() const
{
  return m_hint;
}

uint64_t import_entry::name_rva() const
{
  return m_name_rva;
}

uint64_t import_entry::address_rva() const
{
  return m_address_rva;
}

void import_entry::lib(const std::string &lib)
{
  m_lib = lib;
}

void import_entry::name(const std::string &name)
{
  m_name = name;
}

void import_entry::address(const uintptr_t address)
{
  m_address = address;
}

void import_entry::hint(const uint16_t hint)
{
  m_hint = hint;
}

void import_entry::name_rva(const uint64_t name_rva)
{
  m_name_rva = name_rva;
}

void import_entry::address_rva(const uint64_t address_rva)
{
  m_address_rva = address_rva;
}


}// namespace llama_debug
