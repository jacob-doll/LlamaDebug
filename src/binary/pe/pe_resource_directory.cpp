#include <iomanip>

#include "llama_debug/binary/pe/pe_resource_directory.h"
#include "llama_debug/binary/pe/pe_resource_directory_entry.h"
#include "llama_debug/binary/pe/pe_resource_data_entry.h"

namespace llama_debug {

pe_resource_directory::pe_resource_directory()
  : m_characteristics{ 0x00000000 },
    m_time_date_stamp{ 0x00000000 },
    m_major_version{ 0x0000 },
    m_minor_version{ 0x0000 },
    m_number_of_named_entries{ 0x0000 },
    m_number_of_id_entries{ 0x0000 },
    m_entries{}
{}

pe_resource_directory::pe_resource_directory(const raw_resource_directory *data)
  : m_characteristics{ data->characteristics },
    m_time_date_stamp{ data->time_date_stamp },
    m_major_version{ data->major_version },
    m_minor_version{ data->minor_version },
    m_number_of_named_entries{ data->number_of_named_entries },
    m_number_of_id_entries{ data->number_of_id_entries },
    m_entries{}
{}

pe_resource_directory::pe_resource_directory(const raw_resource_directory *data,
  const std::vector<pe_resource_directory_entry> &entries)
  : m_characteristics{ data->characteristics },
    m_time_date_stamp{ data->time_date_stamp },
    m_major_version{ data->major_version },
    m_minor_version{ data->minor_version },
    m_number_of_named_entries{ data->number_of_named_entries },
    m_number_of_id_entries{ data->number_of_id_entries },
    m_entries{ entries }
{}

uint32_t pe_resource_directory::characteristics() const
{
  return m_characteristics;
}

uint32_t pe_resource_directory::time_date_stamp() const
{
  return m_time_date_stamp;
}

uint16_t pe_resource_directory::major_version() const
{
  return m_major_version;
}

uint16_t pe_resource_directory::minor_version() const
{
  return m_minor_version;
}

uint16_t pe_resource_directory::number_of_named_entries() const
{
  return m_number_of_named_entries;
}

uint16_t pe_resource_directory::number_of_id_entries() const
{
  return m_number_of_id_entries;
}

std::vector<pe_resource_directory_entry> pe_resource_directory::entries() const
{
  return m_entries;
}

void pe_resource_directory::characteristics(const uint32_t characteristics)
{
  m_characteristics = characteristics;
}

void pe_resource_directory::time_date_stamp(const uint32_t time_date_stamp)
{
  m_time_date_stamp = time_date_stamp;
}

void pe_resource_directory::major_version(const uint16_t major_version)
{
  m_major_version = major_version;
}

void pe_resource_directory::minor_version(const uint16_t minor_version)
{
  m_minor_version = minor_version;
}

void pe_resource_directory::number_of_named_entries(const uint16_t number_of_named_entries)
{
  m_number_of_named_entries = number_of_named_entries;
}

void pe_resource_directory::number_of_id_entries(const uint16_t number_of_id_entries)
{
  m_number_of_id_entries = number_of_id_entries;
}

void pe_resource_directory::entries(const std::vector<pe_resource_directory_entry> &entries)
{
  m_entries = entries;
}

void pe_resource_directory::add_entry(const pe_resource_directory_entry &entry)
{
  m_entries.emplace_back(entry);
}

std::ostream &operator<<(std::ostream &os, const pe_resource_directory &directory)
{
  std::ios::fmtflags old_settings = os.flags();

  os << std::hex;

  for (auto &resource_entry : directory.m_entries) {
    os << std::setfill('-') << std::setw(96) << "\n";
    os << "ID: " << resource_entry.id() << "\n";
    os << "Offset to data: " << resource_entry.offset_to_directory() << "\n";
    if (resource_entry.directory()) {
      os << *resource_entry.directory();
    } else {
      os << *resource_entry.data_entry();
    }
  }

  os.flags(old_settings);
  return os;
}

}// namespace llama_debug
