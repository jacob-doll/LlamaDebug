#ifndef LLAMADEBUG_PE_EXPORT_DIRECTORY_H
#define LLAMADEBUG_PE_EXPORT_DIRECTORY_H

#include <cstdint>
#include <vector>
#include <string>

#include "llama_debug/binary/pe/types.h"
#include "llama_debug/binary/pe/pe_export_entry.h"

namespace llama_debug {

struct raw_export_directory
{
  uint32_t characteristics;// reserved
  uint32_t time_date_stamp;
  uint16_t major_version;
  uint16_t minor_version;
  uint32_t name_rva;
  uint32_t ordinal_base;
  uint32_t number_of_functions;
  uint32_t number_of_names;
  uint32_t address_table_rva;
  uint32_t name_pointer_rva;
  uint32_t ordinal_table_rva;
};

class pe_export_directory
{
public:
  pe_export_directory();
  pe_export_directory(const raw_export_directory *data,
    const std::string &name);
  pe_export_directory(const raw_export_directory *data,
    const std::string &name,
    const export_entries_t &export_entries);

  uint32_t characteristics() const;
  uint32_t time_date_stamp() const;
  uint16_t major_version() const;
  uint16_t minor_version() const;
  uint32_t name_rva() const;
  uint32_t ordinal_base() const;
  uint32_t number_of_functions() const;
  uint32_t number_of_names() const;
  uint32_t address_table_rva() const;
  uint32_t name_pointer_rva() const;
  uint32_t ordinal_table_rva() const;
  std::string name() const;
  export_entries_t export_entries() const;

  void characteristics(const uint32_t characteristics);
  void time_date_stamp(const uint32_t time_date_stamp);
  void major_version(const uint16_t major_version);
  void minor_version(const uint16_t minor_version);
  void name_rva(const uint32_t name_rva);
  void ordinal_base(const uint32_t ordinal_base);
  void number_of_functions(const uint32_t number_of_functions);
  void number_of_names(const uint32_t number_of_names);
  void address_table_rva(const uint32_t address_table_rva);
  void name_pointer_rva(const uint32_t name_pointer_rva);
  void ordinal_table_rva(const uint32_t ordinal_table_rva);
  void name(const std::string &name);
  void export_entries(const export_entries_t &export_entries);

  void add_export_entry(export_entry_ptr entry);

  friend std::ostream &operator<<(std::ostream &os, const pe_export_directory &export_directory);

private:
  uint32_t m_characteristics;
  uint32_t m_time_date_stamp;
  uint16_t m_major_version;
  uint16_t m_minor_version;
  uint32_t m_name_rva;
  uint32_t m_ordinal_base;
  uint32_t m_number_of_functions;
  uint32_t m_number_of_names;
  uint32_t m_address_table_rva;
  uint32_t m_name_pointer_rva;
  uint32_t m_ordinal_table_rva;
  std::string m_name;
  export_entries_t m_export_entries;
};

}// namespace llama_debug


#endif// LLAMADEBUG_PE_EXPORT_DIRECTORY_H
