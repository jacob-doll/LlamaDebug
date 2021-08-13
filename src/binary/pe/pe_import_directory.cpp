#include "llama_debug/binary/pe/pe_import_directory.h"

namespace llama_debug {

pe_import_directory::pe_import_directory()
  : m_import_lookup_table_rva{ 0x00000000 },
    m_time_date_stamp{ 0x00000000 },
    m_forwarder_chain{ 0x00000000 },
    m_name_rva{ 0x00000000 },
    m_import_address_table_rva{ 0x00000000 },
    m_name{},
    m_import_entries{}
{}

pe_import_directory::pe_import_directory(const raw_import_directory *data,
  const std::string &name)
  : m_import_lookup_table_rva{ data->import_lookup_table_rva },
    m_time_date_stamp{ data->time_date_stamp },
    m_forwarder_chain{ data->forwarder_chain },
    m_name_rva{ data->name_rva },
    m_import_address_table_rva{ data->import_address_table_rva },
    m_name{ name },
    m_import_entries{}
{}

pe_import_directory::pe_import_directory(const raw_import_directory *data,
  const std::string &name,
  const import_entries_t &import_entries)
  : m_import_lookup_table_rva{ data->import_lookup_table_rva },
    m_time_date_stamp{ data->time_date_stamp },
    m_forwarder_chain{ data->forwarder_chain },
    m_name_rva{ data->name_rva },
    m_import_address_table_rva{ data->import_address_table_rva },
    m_name{ name },
    m_import_entries{ import_entries }
{}

uint32_t pe_import_directory::import_lookup_table_rva() const
{
  return m_import_lookup_table_rva;
}

uint32_t pe_import_directory::time_date_stamp() const
{
  return m_time_date_stamp;
}

uint32_t pe_import_directory::forwarder_chain() const
{
  return m_forwarder_chain;
}

uint32_t pe_import_directory::name_rva() const
{
  return m_name_rva;
}

uint32_t pe_import_directory::import_address_table_rva() const
{
  return m_import_address_table_rva;
}

std::string &pe_import_directory::name()
{
  return m_name;
}

std::vector<pe_import_entry> &pe_import_directory::import_entries()
{
  return m_import_entries;
}

void pe_import_directory::import_lookup_table_rva(const uint32_t import_lookup_table_rva)
{
  m_import_lookup_table_rva = import_lookup_table_rva;
}

void pe_import_directory::time_date_stamp(const uint32_t time_date_stamp)
{
  m_time_date_stamp = time_date_stamp;
}

void pe_import_directory::forwarder_chain(const uint32_t forwarder_chain)
{
  m_forwarder_chain = forwarder_chain;
}

void pe_import_directory::name_rva(const uint32_t name_rva)
{
  m_name_rva = name_rva;
}

void pe_import_directory::import_address_table_rva(const uint32_t import_address_table_rva)
{
  m_import_address_table_rva = import_address_table_rva;
}

void pe_import_directory::name(const std::string &name)
{
  m_name = name;
}

void pe_import_directory::import_entries(const std::vector<pe_import_entry> &import_entries)
{
  m_import_entries = import_entries;
}

void pe_import_directory::add_import_entry(const pe_import_entry &entry)
{
  m_import_entries.emplace_back(entry);
}

}// namespace llama_debug
