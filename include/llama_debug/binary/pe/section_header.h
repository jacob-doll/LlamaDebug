#ifndef LLAMADEBUG_SECTION_HEADER_H
#define LLAMADEBUG_SECTION_HEADER_H

#include <cstdint>
#include <string_view>

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

class section_header
{
public:
  section_header();
  section_header(const raw_section_header *data);

  std::string_view name() const;
  uint32_t virtual_size() const;
  uint32_t virtual_address() const;
  uint32_t size_of_raw_data() const;
  uint32_t pointer_to_raw_data() const;
  uint32_t pointer_to_relocations() const;
  uint32_t pointer_to_line_numbers() const;
  uint16_t number_of_relocations() const;
  uint16_t number_of_line_numbers() const;
  uint32_t characteristics() const;

  void name(const std::string_view &name);
  void virtual_size(const uint32_t virtual_size);
  void virtual_address(const uint32_t virtual_address);
  void size_of_raw_data(const uint32_t size_of_raw_data);
  void pointer_to_raw_data(const uint32_t pointer_to_raw_data);
  void pointer_to_relocations(const uint32_t pointer_to_relocations);
  void pointer_to_line_numbers(const uint32_t pointer_to_line_numbers);
  void number_of_relocations(const uint16_t number_of_relocations);
  void number_of_line_numbers(const uint16_t number_of_line_numbers);
  void characteristics(const uint32_t characteristics);

private:
  std::string_view m_name;
  uint32_t m_virtual_size;
  uint32_t m_virtual_address;
  uint32_t m_size_of_raw_data;
  uint32_t m_pointer_to_raw_data;
  uint32_t m_pointer_to_relocations;
  uint32_t m_pointer_to_line_numbers;
  uint16_t m_number_of_relocations;
  uint16_t m_number_of_line_numbers;
  uint32_t m_characteristics;
};

}// namespace llama_debug

#endif// LLAMADEBUG_SECTION_HEADER_H
