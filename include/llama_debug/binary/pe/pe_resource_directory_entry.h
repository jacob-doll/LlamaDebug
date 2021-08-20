#ifndef LLAMADEBUG_PE_RESOURCE_DIRECTORY_ENTRY_H
#define LLAMADEBUG_PE_RESOURCE_DIRECTORY_ENTRY_H

#include <cstdint>

#include "llama_debug/binary/pe/pe_resource_directory.h"
#include "llama_debug/binary/pe/pe_resource_data_entry.h"

namespace llama_debug {

struct raw_resource_directory_entry
{
  union {
    uint32_t name;
    uint32_t id;
  };
  union {
    uint32_t offset_to_data;
    uint32_t offset_to_directory;
  };
};

class pe_resource_directory_entry
{
public:
  pe_resource_directory_entry();
  pe_resource_directory_entry(const raw_resource_directory_entry *data);

  uint32_t name() const;
  uint32_t id() const;
  uint32_t offset_to_data() const;
  uint32_t offset_to_directory() const;
  pe_resource_directory *directory() const;
  pe_resource_data_entry *data_entry() const;

  void name(const uint32_t name);
  void id(const uint32_t id);
  void offset_to_data(const uint32_t offset_to_data);
  void offset_to_directory(const uint32_t offset_to_directory);
  void directory(pe_resource_directory *directory);
  void data_entry(pe_resource_data_entry *data_entry);

  bool is_name();
  bool is_directory_offset();

private:
  union {
    uint32_t m_name;
    uint32_t m_id;
  };
  union {
    uint32_t m_offset_to_data;
    uint32_t m_offset_to_directory;
  };
  // TODO: use shared ptrs;
  pe_resource_directory *m_directory{ nullptr };
  pe_resource_data_entry *m_data_entry{ nullptr };
};

}// namespace llama_debug


#endif// LLAMADEBUG_PE_RESOURCE_DIRECTORY_ENTRY_H
