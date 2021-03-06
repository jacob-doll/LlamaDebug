#include <iomanip>

#include "llama_debug/binary/pe/pe_optional_header.h"
#include "llama_debug/binary/pe/defs.h"

namespace llama_debug {

pe_optional_header::pe_optional_header()
  : m_magic{ 0x0000 },
    m_major_linker_version{ 0x00 },
    m_minor_linker_version{ 0x00 },
    m_size_of_code{ 0x00000000 },
    m_size_of_initialized_data{ 0x00000000 },
    m_size_of_uninitialized_data{ 0x00000000 },
    m_address_of_entry_point{ 0x00000000 },
    m_base_of_code{ 0x00000000 },
    m_base_of_data{ 0x00000000 },
    m_image_base{ 0x0000000140000000 },
    m_section_alignment{ 0x00001000 },
    m_file_alignment{ 0x00000200 },
    m_major_operating_system_version{ 0x0000 },
    m_minor_operating_system_version{ 0x0000 },
    m_major_image_version{ 0x0000 },
    m_minor_image_version{ 0x0000 },
    m_major_subsystem_version{ 0x0000 },
    m_minor_subsystem_version{ 0x0000 },
    m_win32_version_value{ 0x00000000 },
    m_size_of_image{ 0x00000000 },
    m_size_of_headers{ 0x00000000 },
    m_checksum{ 0x00000000 },
    m_subsystem{ 0x00000000 },
    m_dll_characteristics{},
    m_size_of_stack_reserve{ 0x0000000000000000 },
    m_size_of_stack_commit{ 0x0000000000000000 },
    m_size_of_heap_reserve{ 0x0000000000000000 },
    m_size_of_heap_commit{ 0x0000000000000000 },
    m_loader_flags{ 0x00000000 },
    m_number_of_rva_and_sizes{ 16 },
    m_data_directories{}
{}

pe_optional_header::pe_optional_header(const raw_optional_header32 *data)
  : m_magic{ data->magic },
    m_major_linker_version{ data->major_linker_version },
    m_minor_linker_version{ data->minor_linker_version },
    m_size_of_code{ data->size_of_code },
    m_size_of_initialized_data{ data->size_of_initialized_data },
    m_size_of_uninitialized_data{ data->size_of_uninitialized_data },
    m_address_of_entry_point{ data->address_of_entry_point },
    m_base_of_code{ data->base_of_code },
    m_base_of_data{ data->base_of_data },
    m_image_base{ data->image_base },
    m_section_alignment{ data->section_alignment },
    m_file_alignment{ data->file_alignment },
    m_major_operating_system_version{ data->major_operating_system_version },
    m_minor_operating_system_version{ data->minor_operating_system_version },
    m_major_image_version{ data->major_image_version },
    m_minor_image_version{ data->minor_image_version },
    m_major_subsystem_version{ data->major_subsystem_version },
    m_minor_subsystem_version{ data->minor_subsystem_version },
    m_win32_version_value{ data->win32_version_value },
    m_size_of_image{ data->size_of_image },
    m_size_of_headers{ data->size_of_headers },
    m_checksum{ data->checksum },
    m_subsystem{ data->subsystem },
    m_dll_characteristics{},
    m_size_of_stack_reserve{ data->size_of_stack_reserve },
    m_size_of_stack_commit{ data->size_of_stack_commit },
    m_size_of_heap_reserve{ data->size_of_heap_reserve },
    m_size_of_heap_commit{ data->size_of_heap_commit },
    m_loader_flags{ data->loader_flags },
    m_number_of_rva_and_sizes{ data->number_of_rva_and_sizes },
    m_data_directories{}
{
  std::copy(
    std::begin(data->data_directories),
    std::end(data->data_directories),
    m_data_directories.begin());

  for (auto c : dll_characteristic_array()) {
    uint16_t c_int = c;
    if (data->dll_characteristics & c_int) {
      m_dll_characteristics.insert(c);
    }
  }
}

pe_optional_header::pe_optional_header(const raw_optional_header64 *data)
  : m_magic{ data->magic },
    m_major_linker_version{ data->major_linker_version },
    m_minor_linker_version{ data->minor_linker_version },
    m_size_of_code{ data->size_of_code },
    m_size_of_initialized_data{ data->size_of_initialized_data },
    m_size_of_uninitialized_data{ data->size_of_uninitialized_data },
    m_address_of_entry_point{ data->address_of_entry_point },
    m_base_of_code{ data->base_of_code },
    m_base_of_data{ 0x00000000 },
    m_image_base{ data->image_base },
    m_section_alignment{ data->section_alignment },
    m_file_alignment{ data->file_alignment },
    m_major_operating_system_version{ data->major_operating_system_version },
    m_minor_operating_system_version{ data->minor_operating_system_version },
    m_major_image_version{ data->major_image_version },
    m_minor_image_version{ data->minor_image_version },
    m_major_subsystem_version{ data->major_subsystem_version },
    m_minor_subsystem_version{ data->minor_subsystem_version },
    m_win32_version_value{ data->win32_version_value },
    m_size_of_image{ data->size_of_image },
    m_size_of_headers{ data->size_of_headers },
    m_checksum{ data->checksum },
    m_subsystem{ data->subsystem },
    m_dll_characteristics{},
    m_size_of_stack_reserve{ data->size_of_stack_reserve },
    m_size_of_stack_commit{ data->size_of_stack_commit },
    m_size_of_heap_reserve{ data->size_of_heap_reserve },
    m_size_of_heap_commit{ data->size_of_heap_commit },
    m_loader_flags{ data->loader_flags },
    m_number_of_rva_and_sizes{ data->number_of_rva_and_sizes },
    m_data_directories{}
{
  std::copy(
    std::begin(data->data_directories),
    std::end(data->data_directories),
    m_data_directories.begin());
  for (auto c : dll_characteristic_array()) {
    uint16_t c_int = c;
    if (data->dll_characteristics & c_int) {
      m_dll_characteristics.insert(c);
    }
  }
}

magic_t pe_optional_header::magic() const
{
  return m_magic;
}

uint8_t pe_optional_header::major_linker_version() const
{
  return m_major_linker_version;
}

uint8_t pe_optional_header::minor_linker_version() const
{
  return m_minor_linker_version;
}

uint32_t pe_optional_header::size_of_code() const
{
  return m_size_of_code;
}

uint32_t pe_optional_header::size_of_initialized_data() const
{
  return m_size_of_initialized_data;
}

uint32_t pe_optional_header::size_of_uninitialized_data() const
{
  return m_size_of_uninitialized_data;
}

uint32_t pe_optional_header::address_of_entry_point() const
{
  return m_address_of_entry_point;
}

uint32_t pe_optional_header::base_of_code() const
{
  return m_base_of_code;
}

uint32_t pe_optional_header::base_of_data() const
{
  return m_base_of_data;
}

uint64_t pe_optional_header::image_base() const
{
  return m_image_base;
}

uint32_t pe_optional_header::section_alignment() const
{
  return m_section_alignment;
}

uint32_t pe_optional_header::file_alignment() const
{
  return m_file_alignment;
}

uint16_t pe_optional_header::major_operating_system_version() const
{
  return m_major_operating_system_version;
}

uint16_t pe_optional_header::minor_operating_system_version() const
{
  return m_minor_operating_system_version;
}

uint16_t pe_optional_header::major_image_version() const
{
  return m_major_image_version;
}

uint16_t pe_optional_header::minor_image_version() const
{
  return m_minor_image_version;
}

uint16_t pe_optional_header::major_subsystem_version() const
{
  return m_major_subsystem_version;
}

uint16_t pe_optional_header::minor_subsystem_version() const
{
  return m_minor_subsystem_version;
}

uint32_t pe_optional_header::win32_version_value() const
{
  return m_win32_version_value;
}

uint32_t pe_optional_header::size_of_image() const
{
  return m_size_of_image;
}

uint32_t pe_optional_header::size_of_headers() const
{
  return m_size_of_headers;
}

uint32_t pe_optional_header::checksum() const
{
  return m_checksum;
}

subsystem_t pe_optional_header::subsystem() const
{
  return m_subsystem;
}

std::set<dll_characteristic_t> &pe_optional_header::dll_characteristics()
{
  return m_dll_characteristics;
}

uint64_t pe_optional_header::size_of_stack_reserve() const
{
  return m_size_of_stack_reserve;
}

uint64_t pe_optional_header::size_of_stack_commit() const
{
  return m_size_of_stack_commit;
}

uint64_t pe_optional_header::size_of_heap_reserve() const
{
  return m_size_of_heap_reserve;
}

uint64_t pe_optional_header::size_of_heap_commit() const
{
  return m_size_of_heap_commit;
}

uint32_t pe_optional_header::loader_flags() const
{
  return m_loader_flags;
}

uint32_t pe_optional_header::number_of_rva_and_sizes() const
{
  return m_number_of_rva_and_sizes;
}

std::array<data_directory, IMAGE_NUMBEROF_DIRECTORY_ENTRIES> &pe_optional_header::data_directories()
{
  return m_data_directories;
}

void pe_optional_header::magic(const magic_t magic)
{
  m_magic = magic;
}

void pe_optional_header::major_linker_version(const uint8_t major_linker_version)
{
  m_major_linker_version = major_linker_version;
}

void pe_optional_header::minor_linker_version(const uint8_t minor_linker_version)
{
  m_minor_linker_version = minor_linker_version;
}

void pe_optional_header::size_of_code(const uint32_t size_of_code)
{
  m_size_of_code = size_of_code;
}

void pe_optional_header::size_of_initialized_data(const uint32_t size_of_initialized_data)
{
  m_size_of_initialized_data = size_of_initialized_data;
}

void pe_optional_header::size_of_uninitialized_data(const uint32_t size_of_uninitialized_data)
{
  m_size_of_uninitialized_data = size_of_uninitialized_data;
}

void pe_optional_header::address_of_entry_point(const uint32_t address_of_entry_point)
{
  m_address_of_entry_point = address_of_entry_point;
}

void pe_optional_header::base_of_code(const uint32_t base_of_code)
{
  m_base_of_code = base_of_code;
}

void pe_optional_header::base_of_data(const uint32_t base_of_data)
{
  m_base_of_data = base_of_data;
}

void pe_optional_header::image_base(const uint64_t image_base)
{
  m_image_base = image_base;
}

void pe_optional_header::section_alignment(const uint32_t section_alignment)
{
  m_section_alignment = section_alignment;
}

void pe_optional_header::file_alignment(const uint32_t file_alignment)
{
  m_file_alignment = file_alignment;
}

void pe_optional_header::major_operating_system_version(const uint16_t major_operating_system_version)
{
  m_major_operating_system_version = major_operating_system_version;
}

void pe_optional_header::minor_operating_system_version(const uint16_t minor_operating_system_version)
{
  m_minor_operating_system_version = minor_operating_system_version;
}

void pe_optional_header::major_image_version(const uint16_t major_image_version)
{
  m_major_image_version = major_image_version;
}

void pe_optional_header::minor_image_version(const uint16_t minor_image_version)
{
  m_minor_image_version = minor_image_version;
}

void pe_optional_header::major_subsystem_version(const uint16_t major_subsystem_version)
{
  m_major_subsystem_version = major_subsystem_version;
}

void pe_optional_header::minor_subsystem_version(const uint16_t minor_subsystem_version)
{
  m_minor_subsystem_version = minor_subsystem_version;
}

void pe_optional_header::win32_version_value(const uint32_t win32_version_value)
{
  m_win32_version_value = win32_version_value;
}

void pe_optional_header::size_of_image(const uint32_t size_of_image)
{
  m_size_of_image = size_of_image;
}

void pe_optional_header::size_of_headers(const uint32_t size_of_headers)
{
  m_size_of_headers = size_of_headers;
}

void pe_optional_header::checksum(const uint32_t checksum)
{
  m_checksum = checksum;
}

void pe_optional_header::subsystem(const subsystem_t subsystem)
{
  m_subsystem = subsystem;
}

void pe_optional_header::dll_characteristics(const std::set<dll_characteristic_t> &dll_characteristics)
{
  m_dll_characteristics = dll_characteristics;
}

void pe_optional_header::size_of_stack_reserve(const uint64_t size_of_stack_reserve)
{
  m_size_of_stack_reserve = size_of_stack_reserve;
}

void pe_optional_header::size_of_stack_commit(const uint64_t size_of_stack_commit)
{
  m_size_of_stack_commit = size_of_stack_commit;
}

void pe_optional_header::size_of_heap_reserve(const uint64_t size_of_heap_reserve)
{
  m_size_of_heap_reserve = size_of_heap_reserve;
}

void pe_optional_header::size_of_heap_commit(const uint64_t size_of_heap_commit)
{
  m_size_of_heap_commit = size_of_heap_commit;
}

void pe_optional_header::loader_flags(const uint32_t loader_flags)
{
  m_loader_flags = loader_flags;
}

void pe_optional_header::number_of_rva_and_sizes(const uint32_t number_of_rva_and_sizes)
{
  m_number_of_rva_and_sizes = number_of_rva_and_sizes;
}

void pe_optional_header::data_directories(const std::array<data_directory, IMAGE_NUMBEROF_DIRECTORY_ENTRIES> &data_directories)
{
  m_data_directories = data_directories;
}

std::ostream &operator<<(std::ostream &os, const pe_optional_header &header)
{
  std::ios::fmtflags old_settings = os.flags();

  os << std::hex;
  os << std::left << std::setw(48) << std::setfill(' ') << "Magic: " << magic_string(header.m_magic) << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Major Linker Version: " << header.m_major_linker_version << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Minor Linker Version: " << header.m_minor_linker_version << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Size of Code: " << header.m_size_of_code << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Size of Initialized Data: " << header.m_size_of_initialized_data << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Size of Uninitialized Data: " << header.m_size_of_uninitialized_data << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Address of Entry Point: " << header.m_address_of_entry_point << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Base of Code: " << header.m_base_of_code << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Base of Data: " << header.m_base_of_data << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Image Base: " << header.m_image_base << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Section Alignment: " << header.m_section_alignment << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "File Alignment: " << header.m_file_alignment << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Major OS Version: " << header.m_major_operating_system_version << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Minor OS Version: " << header.m_minor_operating_system_version << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Major Image Version: " << header.m_major_image_version << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Minor Image Version: " << header.m_minor_image_version << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Major Subsystem Version: " << header.m_major_subsystem_version << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Minor Subsystem Version: " << header.m_minor_subsystem_version << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Win32 Version Value: " << header.m_win32_version_value << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Size of Image: " << header.m_size_of_image << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Size of Headers: " << header.m_size_of_headers << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Checksum: " << header.m_checksum << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Subsystem: " << subsystem_string(header.m_subsystem) << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "DLL Characteristics: ";
  auto it = header.m_dll_characteristics.begin();
  auto end = header.m_dll_characteristics.end();
  for (; it != end; ++it) {
    if (it != header.m_dll_characteristics.begin()) {
      os << std::left << std::setw(48) << std::setfill(' ') << " ";
    }
    os << dll_characteristic_string(*it) << "\n";
  }

  os << std::left << std::setw(48) << std::setfill(' ') << "Size of Stack Reserve: " << header.m_size_of_stack_reserve << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Size of Stack Commit: " << header.m_size_of_stack_commit << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Size of Heap Reserve: " << header.m_size_of_heap_reserve << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Size of Heap Commit: " << header.m_size_of_heap_commit << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Loader Flags: " << header.m_loader_flags << "\n";
  os << std::left << std::setw(48) << std::setfill(' ') << "Number of Data Directories: " << header.m_number_of_rva_and_sizes << "\n";

  for (int i = 0; i < header.m_data_directories.size(); i++) {
    os << std::setfill('-') << std::setw(96) << "";
    os << "\n";
    os << directory_entry_string(static_cast<directory_entry_t>(i)) << "\n";
    data_directory dir = header.m_data_directories.at(i);
    os << std::left << std::setw(48) << std::setfill(' ') << "Virtual Adress: " << dir.virtual_address << "\n";
    os << std::left << std::setw(48) << std::setfill(' ') << "Size: " << dir.size;
    if (i < header.m_data_directories.size() - 1) {
      os << "\n";
    }
  }
  // os << std::left << std::setw(48) << std::setfill(' ') << "xxxx: " << header.m_data_directories << "\n";

  os.flags(old_settings);
  return os;
}

}// namespace llama_debug
