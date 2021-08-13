#ifndef LLAMADEBUG_SECTION_HEADER_H
#define LLAMADEBUG_SECTION_HEADER_H

#include <cstdint>
#include <set>

#include "llama_debug/binary/section.h"
#include "llama_debug/binary/pe/defs.h"

namespace llama_debug {

struct raw_section_header
{
  uint8_t name[8];
  uint32_t virtual_size;
  uint32_t virtual_address;
  uint32_t size_of_raw_data;
  uint32_t pointer_to_raw_data;
  uint32_t pointer_to_relocations;
  uint32_t pointer_to_line_numbers;
  uint16_t number_of_relocations;
  uint16_t number_of_line_numbers;
  uint32_t characteristics;
};

class section_header : public section
{
public:
  using section_characteristics_t = std::set<section_characteristic_t>;

  section_header();
  section_header(const raw_section_header *data);

  virtual std::string &name() override;
  virtual uint32_t virtual_size() const override;
  virtual uintptr_t virtual_address() const override;
  virtual uint32_t physical_size() const override;
  virtual uintptr_t physical_address() const override;

  uint32_t pointer_to_relocations() const;
  uint32_t pointer_to_line_numbers() const;
  uint16_t number_of_relocations() const;
  uint16_t number_of_line_numbers() const;
  section_characteristics_t &characteristics();

  void name(const std::string_view &name);
  void virtual_size(const uint32_t virtual_size);
  void virtual_address(const uint32_t virtual_address);
  void size_of_raw_data(const uint32_t size_of_raw_data);
  void pointer_to_raw_data(const uint32_t pointer_to_raw_data);
  void pointer_to_relocations(const uint32_t pointer_to_relocations);
  void pointer_to_line_numbers(const uint32_t pointer_to_line_numbers);
  void number_of_relocations(const uint16_t number_of_relocations);
  void number_of_line_numbers(const uint16_t number_of_line_numbers);
  void characteristics(const section_characteristics_t &characteristics);

  virtual std::ostream &print(std::ostream &os) const override;

private:
  std::string m_name;
  uint32_t m_virtual_size;
  uint32_t m_virtual_address;
  uint32_t m_size_of_raw_data;
  uint32_t m_pointer_to_raw_data;
  uint32_t m_pointer_to_relocations;
  uint32_t m_pointer_to_line_numbers;
  uint16_t m_number_of_relocations;
  uint16_t m_number_of_line_numbers;
  section_characteristics_t m_characteristics;
};

}// namespace llama_debug

#endif// LLAMADEBUG_SECTION_HEADER_H
