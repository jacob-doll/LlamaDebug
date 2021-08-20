#include "llama_debug/binary/pe/pe_resource_data_entry.h"

namespace llama_debug {

pe_resource_data_entry::pe_resource_data_entry()
  : m_offset_to_data{ 0x00000000 },
    m_size{ 0x00000000 },
    m_code_page{ 0x00000000 },
    m_reserved{ 0x00000000 }
{}

pe_resource_data_entry::pe_resource_data_entry(const raw_resource_data_entry *data)
  : m_offset_to_data{ data->offset_to_data },
    m_size{ data->size },
    m_code_page{ data->code_page },
    m_reserved{ data->reserved }
{}

uint32_t pe_resource_data_entry::offset_to_data() const
{
  return m_offset_to_data;
}

uint32_t pe_resource_data_entry::size() const
{
  return m_size;
}

uint32_t pe_resource_data_entry::code_page() const
{
  return m_code_page;
}

uint32_t pe_resource_data_entry::reserved() const
{
  return m_reserved;
}

void pe_resource_data_entry::offset_to_data(const uint32_t offset_to_data)
{
  m_offset_to_data = offset_to_data;
}

void pe_resource_data_entry::size(const uint32_t size)
{
  m_size = size;
}

void pe_resource_data_entry::code_page(const uint32_t code_page)
{
  m_code_page = code_page;
}

void pe_resource_data_entry::reserved(const uint32_t reserved)
{
  m_reserved = reserved;
}

}// namespace llama_debug
