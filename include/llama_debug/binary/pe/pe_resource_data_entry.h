#ifndef LLAMADEBUG_PE_RESOURCE_DATA_ENTRY_H
#define LLAMADEBUG_PE_RESOURCE_DATA_ENTRY_H

#include <cstdint>
#include <ostream>

namespace llama_debug {

struct raw_resource_data_entry
{
  uint32_t offset_to_data;
  uint32_t size;
  uint32_t code_page;
  uint32_t reserved;
};

class pe_resource_data_entry
{
public:
  pe_resource_data_entry();
  pe_resource_data_entry(const raw_resource_data_entry *data);

  uint32_t offset_to_data() const;
  uint32_t size() const;
  uint32_t code_page() const;
  uint32_t reserved() const;

  void offset_to_data(const uint32_t offset_to_data);
  void size(const uint32_t size);
  void code_page(const uint32_t code_page);
  void reserved(const uint32_t reserved);

  friend std::ostream &operator<<(std::ostream &os, const pe_resource_data_entry &data_entry);

private:
  uint32_t m_offset_to_data;
  uint32_t m_size;
  uint32_t m_code_page;
  uint32_t m_reserved;
};

}// namespace llama_debug

#endif// LLAMADEBUG_PE_RESOURCE_DATA_ENTRY_H
