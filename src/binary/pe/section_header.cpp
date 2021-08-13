#include <iomanip>

#include "llama_debug/binary/pe/section_header.h"

namespace llama_debug {

section_header::section_header()
  : m_name{},
    m_virtual_size{ 0x00000000 },
    m_virtual_address{ 0x00000000 },
    m_size_of_raw_data{ 0x00000000 },
    m_pointer_to_raw_data{ 0x00000000 },
    m_pointer_to_relocations{ 0x00000000 },
    m_pointer_to_line_numbers{ 0x00000000 },
    m_number_of_relocations{ 0x0000 },
    m_number_of_line_numbers{ 0x0000 },
    m_characteristics{}
{}

section_header::section_header(const raw_section_header *data)
  : m_name{ (const char *)(&data->name[0]), std::size(data->name) },
    m_virtual_size{ data->virtual_size },
    m_virtual_address{ data->virtual_address },
    m_size_of_raw_data{ data->size_of_raw_data },
    m_pointer_to_raw_data{ data->pointer_to_raw_data },
    m_pointer_to_relocations{ data->pointer_to_relocations },
    m_pointer_to_line_numbers{ data->pointer_to_line_numbers },
    m_number_of_relocations{ data->number_of_relocations },
    m_number_of_line_numbers{ data->number_of_line_numbers },
    m_characteristics{}
{
  for (auto c : section_characteristic_array()) {
    uint32_t c_int = c;
    if (data->characteristics & c_int) {
      m_characteristics.insert(c);
    }
  }
}

std::string section_header::name() const
{
  return m_name;
}

uint32_t section_header::virtual_size() const
{
  return m_virtual_size;
}

uintptr_t section_header::virtual_address() const
{
  return m_virtual_address;
}

uint32_t section_header::physical_size() const
{
  return m_size_of_raw_data;
}

uintptr_t section_header::physical_address() const
{
  return m_pointer_to_raw_data;
}

uint32_t section_header::pointer_to_relocations() const
{
  return m_pointer_to_relocations;
}

uint32_t section_header::pointer_to_line_numbers() const
{
  return m_pointer_to_line_numbers;
}

uint16_t section_header::number_of_relocations() const
{
  return m_number_of_relocations;
}

uint16_t section_header::number_of_line_numbers() const
{
  return m_number_of_line_numbers;
}

std::set<section_characteristic_t> section_header::characteristics() const
{
  return m_characteristics;
}

void section_header::name(const std::string_view &name)
{
  m_name = name;
}

void section_header::virtual_size(const uint32_t virtual_size)
{
  m_virtual_size = virtual_size;
}

void section_header::virtual_address(const uint32_t virtual_address)
{
  m_virtual_address = virtual_address;
}

void section_header::size_of_raw_data(const uint32_t size_of_raw_data)
{
  m_size_of_raw_data = size_of_raw_data;
}

void section_header::pointer_to_raw_data(const uint32_t pointer_to_raw_data)
{
  m_pointer_to_raw_data = pointer_to_raw_data;
}

void section_header::pointer_to_relocations(const uint32_t pointer_to_relocations)
{
  m_pointer_to_relocations = pointer_to_relocations;
}

void section_header::pointer_to_line_numbers(const uint32_t pointer_to_line_numbers)
{
  m_pointer_to_line_numbers = pointer_to_line_numbers;
}

void section_header::number_of_relocations(const uint16_t number_of_relocations)
{
  m_number_of_relocations = number_of_relocations;
}

void section_header::number_of_line_numbers(const uint16_t number_of_line_numbers)
{
  m_number_of_line_numbers = number_of_line_numbers;
}

void section_header::characteristics(const std::set<section_characteristic_t> &characteristics)
{
  m_characteristics = characteristics;
}

std::ostream &section_header::print(std::ostream &os) const
{
  std::ios::fmtflags old_settings = os.flags();

  os << std::left << std::setw(48) << std::setfill(' ') << "Name: " << m_name << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Virtual Size: " << m_virtual_size << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Virtual Address: " << m_virtual_address << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Size of Raw Data: " << m_size_of_raw_data << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Pointer to Raw Data: " << m_pointer_to_raw_data << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Pointer to Relocations: " << m_pointer_to_relocations << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Pointer to Line Numbers: " << m_pointer_to_line_numbers << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Number of Relocations: " << m_number_of_relocations << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Number of Line Numbers: " << m_number_of_line_numbers << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Characteristics: ";
  auto it = m_characteristics.begin();
  auto end = m_characteristics.end();
  for (; it != end; ++it) {
    if (it != m_characteristics.begin()) {
      os << std::left << std::setw(48) << std::setfill(' ') << " ";
    }
    os << section_characteristic_string(*it);
    if (it != --m_characteristics.end()) {
      os << "\n";
    }
  }

  os.flags(old_settings);
  return os;
}

}// namespace llama_debug
