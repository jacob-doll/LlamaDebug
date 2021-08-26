#ifndef LLAMADEBUG_PE_RESOURCE_DIRECTORY_H
#define LLAMADEBUG_PE_RESOURCE_DIRECTORY_H

#include <cstdint>
#include <vector>
#include <ostream>

#include "llama_debug/binary/pe/pe_resource_directory_entry.h"

namespace llama_debug {

struct raw_resource_directory
{
  uint32_t characteristics;
  uint32_t time_date_stamp;
  uint16_t major_version;
  uint16_t minor_version;
  uint16_t number_of_named_entries;
  uint16_t number_of_id_entries;
};

class pe_resource_directory
{
public:
  using resource_directory_entries_t = std::vector<pe_resource_directory_entry>;

  pe_resource_directory();
  pe_resource_directory(const raw_resource_directory *data);
  pe_resource_directory(const raw_resource_directory *data,
    const resource_directory_entries_t &entries);

  uint32_t characteristics() const;
  uint32_t time_date_stamp() const;
  uint16_t major_version() const;
  uint16_t minor_version() const;
  uint16_t number_of_named_entries() const;
  uint16_t number_of_id_entries() const;
  resource_directory_entries_t entries() const;

  void characteristics(const uint32_t characteristics);
  void time_date_stamp(const uint32_t time_date_stamp);
  void major_version(const uint16_t major_version);
  void minor_version(const uint16_t minor_version);
  void number_of_named_entries(const uint16_t number_of_named_entries);
  void number_of_id_entries(const uint16_t number_of_id_entries);
  void entries(const resource_directory_entries_t &entries);

  void add_entry(const pe_resource_directory_entry &entry);

  friend std::ostream &operator<<(std::ostream &os, const pe_resource_directory &directory);

private:
  uint32_t m_characteristics;
  uint32_t m_time_date_stamp;
  uint16_t m_major_version;
  uint16_t m_minor_version;
  uint16_t m_number_of_named_entries;
  uint16_t m_number_of_id_entries;
  resource_directory_entries_t m_entries;
};

}// namespace llama_debug

#endif// LLAMADEBUG_PE_RESOURCE_DIRECTORY_H
