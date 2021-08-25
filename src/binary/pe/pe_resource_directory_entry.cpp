#include "llama_debug/binary/pe/pe_resource_directory_entry.h"

namespace llama_debug {

pe_resource_directory_entry::pe_resource_directory_entry()
  : m_id{ 0x00000000 },
    m_offset_to_directory{ 0x00000000 }
{}
pe_resource_directory_entry::pe_resource_directory_entry(const raw_resource_directory_entry *data)
  : m_id{ data->id },
    m_offset_to_directory{ data->offset_to_directory }
{}

uint32_t pe_resource_directory_entry::name() const
{
  return m_name;
}

uint32_t pe_resource_directory_entry::id() const
{
  return m_id;
}

uint32_t pe_resource_directory_entry::offset_to_data() const
{
  return m_offset_to_data;
}

uint32_t pe_resource_directory_entry::offset_to_directory() const
{
  return m_offset_to_directory;
}

std::shared_ptr<pe_resource_directory> pe_resource_directory_entry::directory() const
{
  return m_directory;
}

std::shared_ptr<pe_resource_data_entry> pe_resource_directory_entry::data_entry() const
{
  return m_data_entry;
}

void pe_resource_directory_entry::name(const uint32_t name)
{
  m_name = name;
}

void pe_resource_directory_entry::id(const uint32_t id)
{
  m_id = id;
}

void pe_resource_directory_entry::offset_to_data(const uint32_t offset_to_data)
{
  m_offset_to_data = offset_to_data;
}

void pe_resource_directory_entry::offset_to_directory(const uint32_t offset_to_directory)
{
  m_offset_to_directory = offset_to_directory;
}

void pe_resource_directory_entry::directory(std::shared_ptr<pe_resource_directory> directory)
{
  m_directory = directory;
}

void pe_resource_directory_entry::data_entry(std::shared_ptr<pe_resource_data_entry> data_entry)
{
  m_data_entry = data_entry;
}


bool pe_resource_directory_entry::is_name()
{
  return m_name & 0x80000000;
}

bool pe_resource_directory_entry::is_directory_offset()
{
  return m_offset_to_directory & 0x80000000;
}

}// namespace llama_debug
