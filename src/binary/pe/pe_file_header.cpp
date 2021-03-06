#include <iomanip>
#include <ctime>

#include "llama_debug/binary/pe/pe_file_header.h"
#include "llama_debug/binary/pe/defs.h"

namespace llama_debug {

pe_file_header::pe_file_header()
  : m_machine{ IMAGE_FILE_MACHINE_UNKNOWN },
    m_number_of_sections{ 0x0000 },
    m_time_date_stamp{ 0x00000000 },
    m_pointer_to_symbol_table{ 0x00000000 },
    m_number_of_symbols{ 0x00000000 },
    m_size_of_optional_header{ 0x0000 },
    m_characteristics{ IMAGE_FILE_EXECUTABLE_IMAGE }
{}

pe_file_header::pe_file_header(const raw_file_header *data)
  : m_machine{ data->machine },
    m_number_of_sections{ data->number_of_sections },
    m_time_date_stamp{ data->time_date_stamp },
    m_pointer_to_symbol_table{ data->pointer_to_symbol_table },
    m_number_of_symbols{ data->number_of_symbols },
    m_size_of_optional_header{ data->size_of_optional_header },
    m_characteristics{}
{
  for (auto c : characteristic_array()) {
    uint16_t c_int = c;
    if (data->characteristics & c_int) {
      m_characteristics.insert(c);
    }
  }
}

machine_t pe_file_header::machine() const
{
  return m_machine;
}

uint16_t pe_file_header::number_of_sections() const
{
  return m_number_of_sections;
}

uint32_t pe_file_header::time_date_stamp() const
{
  return m_time_date_stamp;
}

uint32_t pe_file_header::pointer_to_symbol_table() const
{
  return m_pointer_to_symbol_table;
}

uint32_t pe_file_header::number_of_symbols() const
{
  return m_number_of_symbols;
}

uint16_t pe_file_header::size_of_optional_header() const
{
  return m_size_of_optional_header;
}

std::set<characteristic_t> &pe_file_header::characteristics()
{
  return m_characteristics;
}

void pe_file_header::machine(const machine_t machine)
{
  m_machine = machine;
}

void pe_file_header::number_of_sections(const uint16_t number_of_sections)
{
  m_number_of_sections = number_of_sections;
}

void pe_file_header::time_date_stamp(const uint32_t time_date_stamp)
{
  m_time_date_stamp = time_date_stamp;
}

void pe_file_header::pointer_to_symbol_table(const uint32_t pointer_to_symbol_table)
{
  m_pointer_to_symbol_table = pointer_to_symbol_table;
}

void pe_file_header::number_of_symbols(const uint32_t number_of_symbols)
{
  m_number_of_symbols = number_of_symbols;
}

void pe_file_header::size_of_optional_header(const uint16_t size_of_optional_header)
{
  m_size_of_optional_header = size_of_optional_header;
}

void pe_file_header::characteristics(const characteristics_t &characteristics)
{
  m_characteristics = characteristics;
}

std::ostream &operator<<(std::ostream &os, const pe_file_header &header)
{
  std::ios::fmtflags old_settings = os.flags();

  os << std::hex;
  os << std::left << std::setw(48) << std::setfill(' ') << "Machine: " << machine_string(header.m_machine) << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Number of Sections: " << header.m_number_of_sections << "\n";
  std::time_t time_date_stamp_ = header.m_time_date_stamp;
  os << std::left << std::setw(48) << std::setfill(' ') << "Time Date Stamp: " << std::ctime(&time_date_stamp_);
  os << std::left << std::setw(48) << std::setfill(' ') << "Pointer to Symbol Table: " << header.m_pointer_to_symbol_table << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Number of Symbols: " << header.m_number_of_symbols << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Size of Optional Header: " << header.m_size_of_optional_header << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Characteristics: ";
  auto it = header.m_characteristics.begin();
  auto end = header.m_characteristics.end();
  for (; it != end; ++it) {
    if (it != header.m_characteristics.begin()) {
      os << std::left << std::setw(48) << std::setfill(' ') << " ";
    }
    os << characteristic_string(*it);
    if (it != --header.m_characteristics.end()) {
      os << "\n";
    }
  }

  os.flags(old_settings);
  return os;
}

}// namespace llama_debug
