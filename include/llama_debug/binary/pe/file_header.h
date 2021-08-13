#ifndef LLAMADEBUG_FILE_HEADER_H
#define LLAMADEBUG_FILE_HEADER_H

#include <cstdint>
#include <ostream>
#include <set>

#include "llama_debug/binary/pe/defs.h"

namespace llama_debug {

struct raw_file_header
{
  uint16_t machine;
  uint16_t number_of_sections;
  uint32_t time_date_stamp;
  uint32_t pointer_to_symbol_table;
  uint32_t number_of_symbols;
  uint16_t size_of_optional_header;
  uint16_t characteristics;
};

class file_header
{
public:
  using characteristics_t = std::set<characteristic_t>;

  file_header();
  file_header(const raw_file_header *data);

  machine_t machine() const;
  uint16_t number_of_sections() const;
  uint32_t time_date_stamp() const;
  uint32_t pointer_to_symbol_table() const;
  uint32_t number_of_symbols() const;
  uint16_t size_of_optional_header() const;
  characteristics_t &characteristics();

  void machine(const machine_t machine);
  void number_of_sections(const uint16_t number_of_sections);
  void time_date_stamp(const uint32_t time_date_stamp);
  void pointer_to_symbol_table(const uint32_t pointer_to_symbol_table);
  void number_of_symbols(const uint32_t number_of_symbols);
  void size_of_optional_header(const uint16_t size_of_optional_header);
  void characteristics(const characteristics_t &characteristics);

  friend std::ostream &operator<<(std::ostream &os, const file_header &header);

private:
  machine_t m_machine;
  uint16_t m_number_of_sections;
  uint32_t m_time_date_stamp;
  uint32_t m_pointer_to_symbol_table;
  uint32_t m_number_of_symbols;
  uint16_t m_size_of_optional_header;
  characteristics_t m_characteristics;
};

}// namespace llama_debug


#endif// LLAMADEBUG_FILE_HEADER_H
