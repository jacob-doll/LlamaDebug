#ifndef LLAMADEBUG_IMPORT_DIRECTORY_H
#define LLAMADEBUG_IMPORT_DIRECTORY_H

#include <cstdint>

namespace llama_debug {

struct raw_import_directory
{
  uint32_t import_lookup_table_rva;
  uint32_t time_date_stamp;
  uint32_t forwarder_chain;
  uint32_t name_rva;
  uint32_t import_address_table_rva;
};

class import_directory
{
public:
  import_directory();
  import_directory(const raw_import_directory *data);

  uint32_t import_lookup_table_rva() const;
  uint32_t time_date_stamp() const;
  uint32_t forwarder_chain() const;
  uint32_t name_rva() const;
  uint32_t import_address_table_rva() const;

  void import_lookup_table_rva(const uint32_t import_lookup_table_rva);
  void time_date_stamp(const uint32_t time_date_stamp);
  void forwarder_chain(const uint32_t forwarder_chain);
  void name_rva(const uint32_t name_rva);
  void import_address_table_rva(const uint32_t import_address_table_rva);

private:
  uint32_t m_import_lookup_table_rva;
  uint32_t m_time_date_stamp;
  uint32_t m_forwarder_chain;
  uint32_t m_name_rva;
  uint32_t m_import_address_table_rva;
};

}// namespace llama_debug

#endif// LLAMADEBUG_IMPORT_DIRECTORY_H
