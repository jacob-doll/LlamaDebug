#ifndef LLAMADEBUG_PE_IMPORT_DIRECTORY_H
#define LLAMADEBUG_PE_IMPORT_DIRECTORY_H

#include <cstdint>
#include <vector>

#include "llama_debug/binary/pe/pe_import_entry.h"

namespace llama_debug {

struct raw_import_directory
{
  uint32_t import_lookup_table_rva;
  uint32_t time_date_stamp;
  uint32_t forwarder_chain;
  uint32_t name_rva;
  uint32_t import_address_table_rva;
};

class pe_import_directory
{
public:
  using import_entries_t = std::vector<pe_import_entry>;

  pe_import_directory();
  pe_import_directory(const raw_import_directory *data,
    const std::string &name);
  pe_import_directory(const raw_import_directory *data,
    const std::string &name,
    const import_entries_t &import_entries);

  uint32_t import_lookup_table_rva() const;
  uint32_t time_date_stamp() const;
  uint32_t forwarder_chain() const;
  uint32_t name_rva() const;
  uint32_t import_address_table_rva() const;
  std::string &name();
  import_entries_t &import_entries();

  void import_lookup_table_rva(const uint32_t import_lookup_table_rva);
  void time_date_stamp(const uint32_t time_date_stamp);
  void forwarder_chain(const uint32_t forwarder_chain);
  void name_rva(const uint32_t name_rva);
  void import_address_table_rva(const uint32_t import_address_table_rva);
  void name(const std::string &name);
  void import_entries(const import_entries_t &import_entries);

  void add_import_entry(const pe_import_entry &entry);

private:
  uint32_t m_import_lookup_table_rva;
  uint32_t m_time_date_stamp;
  uint32_t m_forwarder_chain;
  uint32_t m_name_rva;
  uint32_t m_import_address_table_rva;
  std::string m_name;
  import_entries_t m_import_entries;
};

}// namespace llama_debug

#endif// LLAMADEBUG_PE_IMPORT_DIRECTORY_H
