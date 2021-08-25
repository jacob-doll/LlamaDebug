#ifndef LLAMADEBUG_PE_RESOURCE_DIRECTORY_ENTRY_H
#define LLAMADEBUG_PE_RESOURCE_DIRECTORY_ENTRY_H

#include <cstdint>
#include <memory>

namespace llama_debug {

class pe_resource_directory;
class pe_resource_data_entry;

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
  using directory_ptr_t = std::shared_ptr<pe_resource_directory>;
  using data_entry_ptr_t = std::shared_ptr<pe_resource_data_entry>;

  pe_resource_directory_entry();
  pe_resource_directory_entry(const raw_resource_directory_entry *data);

  uint32_t name() const;
  uint32_t id() const;
  uint32_t offset_to_data() const;
  uint32_t offset_to_directory() const;
  directory_ptr_t directory() const;
  data_entry_ptr_t data_entry() const;

  void name(const uint32_t name);
  void id(const uint32_t id);
  void offset_to_data(const uint32_t offset_to_data);
  void offset_to_directory(const uint32_t offset_to_directory);
  void directory(directory_ptr_t directory);
  void data_entry(data_entry_ptr_t data_entry);

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
  directory_ptr_t m_directory;
  data_entry_ptr_t m_data_entry;
};

}// namespace llama_debug


#endif// LLAMADEBUG_PE_RESOURCE_DIRECTORY_ENTRY_H
