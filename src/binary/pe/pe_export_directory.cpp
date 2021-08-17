#include "llama_debug/binary/pe/pe_export_directory.h"

namespace llama_debug {

pe_export_directory::pe_export_directory()
  : m_characteristics{ 0x00000000 },
    m_time_date_stamp{ 0x00000000 },
    m_major_version{ 0x0000 },
    m_minor_version{ 0x0000 },
    m_name_rva{ 0x00000000 },
    m_ordinal_base{ 0x00000000 },
    m_number_of_functions{ 0x00000000 },
    m_number_of_names{ 0x00000000 },
    m_address_table_rva{ 0x00000000 },
    m_name_pointer_rva{ 0x00000000 },
    m_ordinal_table_rva{ 0x00000000 },
    m_name{},
    m_export_entries{}
{}

pe_export_directory::pe_export_directory(const raw_export_directory *data,
  const std::string &name)
  : m_characteristics{ data->characteristics },
    m_time_date_stamp{ data->time_date_stamp },
    m_major_version{ data->major_version },
    m_minor_version{ data->minor_version },
    m_name_rva{ data->name_rva },
    m_ordinal_base{ data->ordinal_base },
    m_number_of_functions{ data->number_of_functions },
    m_number_of_names{ data->number_of_names },
    m_address_table_rva{ data->address_table_rva },
    m_name_pointer_rva{ data->name_pointer_rva },
    m_ordinal_table_rva{ data->ordinal_table_rva },
    m_name{ name },
    m_export_entries{}
{}

pe_export_directory::pe_export_directory(const raw_export_directory *data,
  const std::string &name,
  const export_entries_t &export_entries)
  : m_characteristics{ data->characteristics },
    m_time_date_stamp{ data->time_date_stamp },
    m_major_version{ data->major_version },
    m_minor_version{ data->minor_version },
    m_name_rva{ data->name_rva },
    m_ordinal_base{ data->ordinal_base },
    m_number_of_functions{ data->number_of_functions },
    m_number_of_names{ data->number_of_names },
    m_address_table_rva{ data->address_table_rva },
    m_name_pointer_rva{ data->name_pointer_rva },
    m_ordinal_table_rva{ data->ordinal_table_rva },
    m_name{ name },
    m_export_entries{ export_entries }
{}

uint32_t pe_export_directory::characteristics() const
{
  return m_characteristics;
}

uint32_t pe_export_directory::time_date_stamp() const
{
  return m_time_date_stamp;
}

uint16_t pe_export_directory::major_version() const
{
  return m_major_version;
}

uint16_t pe_export_directory::minor_version() const
{
  return m_minor_version;
}

uint32_t pe_export_directory::name_rva() const
{
  return m_name_rva;
}

uint32_t pe_export_directory::ordinal_base() const
{
  return m_ordinal_base;
}

uint32_t pe_export_directory::number_of_functions() const
{
  return m_number_of_functions;
}

uint32_t pe_export_directory::number_of_names() const
{
  return m_number_of_names;
}

uint32_t pe_export_directory::address_table_rva() const
{
  return m_address_table_rva;
}

uint32_t pe_export_directory::name_pointer_rva() const
{
  return m_name_pointer_rva;
}

uint32_t pe_export_directory::ordinal_table_rva() const
{
  return m_ordinal_table_rva;
}

std::string pe_export_directory::name() const
{
  return m_name;
}

std::vector<pe_export_entry> pe_export_directory::export_entries() const
{
  return m_export_entries;
}

void pe_export_directory::characteristics(const uint32_t characteristics)
{
  m_characteristics = characteristics;
}

void pe_export_directory::time_date_stamp(const uint32_t time_date_stamp)
{
  m_time_date_stamp = time_date_stamp;
}

void pe_export_directory::major_version(const uint16_t major_version)
{
  m_major_version = major_version;
}

void pe_export_directory::minor_version(const uint16_t minor_version)
{
  m_minor_version = minor_version;
}

void pe_export_directory::name_rva(const uint32_t name_rva)
{
  m_name_rva = name_rva;
}

void pe_export_directory::ordinal_base(const uint32_t ordinal_base)
{
  m_ordinal_base = ordinal_base;
}

void pe_export_directory::number_of_functions(const uint32_t number_of_functions)
{
  m_number_of_functions = number_of_functions;
}

void pe_export_directory::number_of_names(const uint32_t number_of_names)
{
  m_number_of_names = number_of_names;
}

void pe_export_directory::address_table_rva(const uint32_t address_table_rva)
{
  m_address_table_rva = address_table_rva;
}

void pe_export_directory::name_pointer_rva(const uint32_t name_pointer_rva)
{
  m_name_pointer_rva = name_pointer_rva;
}

void pe_export_directory::ordinal_table_rva(const uint32_t ordinal_table_rva)
{
  m_ordinal_table_rva = ordinal_table_rva;
}

void pe_export_directory::name(const std::string &name)
{
  m_name = name;
}

void pe_export_directory::export_entries(const std::vector<pe_export_entry> &export_entries)
{
  m_export_entries = export_entries;
}

void pe_export_directory::add_export_entry(const pe_export_entry &entry)
{
  m_export_entries.emplace_back(entry);
}

}// namespace llama_debug
