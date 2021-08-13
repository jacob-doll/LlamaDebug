#ifndef LLAMADEBUG_OPTIONAL_HEADER_H
#define LLAMADEBUG_OPTIONAL_HEADER_H

#include <cstdint>
#include <ostream>
#include <array>
#include <set>

#include "llama_debug/binary/pe/defs.h"

namespace llama_debug {

struct data_directory
{
  uint32_t virtual_address;
  uint32_t size;
};

struct raw_optional_header32
{
  /* Standard Fields */
  uint16_t magic;
  uint8_t major_linker_version;
  uint8_t minor_linker_version;
  uint32_t size_of_code;
  uint32_t size_of_initialized_data;
  uint32_t size_of_uninitialized_data;
  uint32_t address_of_entry_point;
  uint32_t base_of_code;
  uint32_t base_of_data;
  /* Windows-Specific Fields */
  uint32_t image_base;
  uint32_t section_alignment;
  uint32_t file_alignment;
  uint16_t major_operating_system_version;
  uint16_t minor_operating_system_version;
  uint16_t major_image_version;
  uint16_t minor_image_version;
  uint16_t major_subsystem_version;
  uint16_t minor_subsystem_version;
  uint32_t win32_version_value;
  uint32_t size_of_image;
  uint32_t size_of_headers;
  uint32_t checksum;
  uint16_t subsystem;
  uint16_t dll_characteristics;
  uint32_t size_of_stack_reserve;
  uint32_t size_of_stack_commit;
  uint32_t size_of_heap_reserve;
  uint32_t size_of_heap_commit;
  uint32_t loader_flags;
  uint32_t number_of_rva_and_sizes;
  data_directory data_directories[16];
};

struct raw_optional_header64
{
  /* Standard Fields */
  uint16_t magic;
  uint8_t major_linker_version;
  uint8_t minor_linker_version;
  uint32_t size_of_code;
  uint32_t size_of_initialized_data;
  uint32_t size_of_uninitialized_data;
  uint32_t address_of_entry_point;
  uint32_t base_of_code;
  /* Windows-Specific Fields */
  uint64_t image_base;
  uint32_t section_alignment;
  uint32_t file_alignment;
  uint16_t major_operating_system_version;
  uint16_t minor_operating_system_version;
  uint16_t major_image_version;
  uint16_t minor_image_version;
  uint16_t major_subsystem_version;
  uint16_t minor_subsystem_version;
  uint32_t win32_version_value;
  uint32_t size_of_image;
  uint32_t size_of_headers;
  uint32_t checksum;
  uint16_t subsystem;
  uint16_t dll_characteristics;
  uint64_t size_of_stack_reserve;
  uint64_t size_of_stack_commit;
  uint64_t size_of_heap_reserve;
  uint64_t size_of_heap_commit;
  uint32_t loader_flags;
  uint32_t number_of_rva_and_sizes;
  data_directory data_directories[16];
};

class optional_header
{
public:
  using data_directory_t = std::array<data_directory, 16>;
  using dll_characteristics_t = std::set<dll_characteristic_t>;

  optional_header();
  optional_header(const raw_optional_header32 *data);
  optional_header(const raw_optional_header64 *data);

  magic_t magic() const;
  uint8_t major_linker_version() const;
  uint8_t minor_linker_version() const;
  uint32_t size_of_code() const;
  uint32_t size_of_initialized_data() const;
  uint32_t size_of_uninitialized_data() const;
  uint32_t address_of_entry_point() const;
  uint32_t base_of_code() const;
  uint32_t base_of_data() const;
  uint64_t image_base() const;
  uint32_t section_alignment() const;
  uint32_t file_alignment() const;
  uint16_t major_operating_system_version() const;
  uint16_t minor_operating_system_version() const;
  uint16_t major_image_version() const;
  uint16_t minor_image_version() const;
  uint16_t major_subsystem_version() const;
  uint16_t minor_subsystem_version() const;
  uint32_t win32_version_value() const;
  uint32_t size_of_image() const;
  uint32_t size_of_headers() const;
  uint32_t checksum() const;
  subsystem_t subsystem() const;
  dll_characteristics_t dll_characteristics() const;
  uint64_t size_of_stack_reserve() const;
  uint64_t size_of_stack_commit() const;
  uint64_t size_of_heap_reserve() const;
  uint64_t size_of_heap_commit() const;
  uint32_t loader_flags() const;
  uint32_t number_of_rva_and_sizes() const;
  data_directory_t data_directories() const;

  void magic(const magic_t magic);
  void major_linker_version(const uint8_t major_linker_version);
  void minor_linker_version(const uint8_t minor_linker_version);
  void size_of_code(const uint32_t size_of_code);
  void size_of_initialized_data(const uint32_t size_of_initialized_data);
  void size_of_uninitialized_data(const uint32_t size_of_uninitialized_data);
  void address_of_entry_point(const uint32_t address_of_entry_point);
  void base_of_code(const uint32_t base_of_code);
  void base_of_data(const uint32_t base_of_data);
  void image_base(const uint64_t image_base);
  void section_alignment(const uint32_t section_alignment);
  void file_alignment(const uint32_t file_alignment);
  void major_operating_system_version(const uint16_t major_operating_system_version);
  void minor_operating_system_version(const uint16_t minor_operating_system_version);
  void major_image_version(const uint16_t major_image_version);
  void minor_image_version(const uint16_t minor_image_version);
  void major_subsystem_version(const uint16_t major_subsystem_version);
  void minor_subsystem_version(const uint16_t minor_subsystem_version);
  void win32_version_value(const uint32_t win32_version_value);
  void size_of_image(const uint32_t size_of_image);
  void size_of_headers(const uint32_t size_of_headers);
  void checksum(const uint32_t checksum);
  void subsystem(const subsystem_t subsystem);
  void dll_characteristics(const dll_characteristics_t &dll_characteristics);
  void size_of_stack_reserve(const uint64_t size_of_stack_reserve);
  void size_of_stack_commit(const uint64_t size_of_stack_commit);
  void size_of_heap_reserve(const uint64_t size_of_heap_reserve);
  void size_of_heap_commit(const uint64_t size_of_heap_commit);
  void loader_flags(const uint32_t loader_flags);
  void number_of_rva_and_sizes(const uint32_t number_of_rva_and_sizes);
  void data_directories(const data_directory_t &data_directories);

  friend std::ostream &operator<<(std::ostream &os, const optional_header &header);

private:
  magic_t m_magic;
  uint8_t m_major_linker_version;
  uint8_t m_minor_linker_version;
  uint32_t m_size_of_code;
  uint32_t m_size_of_initialized_data;
  uint32_t m_size_of_uninitialized_data;
  uint32_t m_address_of_entry_point;
  uint32_t m_base_of_code;
  uint32_t m_base_of_data;

  uint64_t m_image_base;
  uint32_t m_section_alignment;
  uint32_t m_file_alignment;
  uint16_t m_major_operating_system_version;
  uint16_t m_minor_operating_system_version;
  uint16_t m_major_image_version;
  uint16_t m_minor_image_version;
  uint16_t m_major_subsystem_version;
  uint16_t m_minor_subsystem_version;
  uint32_t m_win32_version_value;
  uint32_t m_size_of_image;
  uint32_t m_size_of_headers;
  uint32_t m_checksum;
  subsystem_t m_subsystem;
  dll_characteristics_t m_dll_characteristics;
  uint64_t m_size_of_stack_reserve;
  uint64_t m_size_of_stack_commit;
  uint64_t m_size_of_heap_reserve;
  uint64_t m_size_of_heap_commit;
  uint32_t m_loader_flags;
  uint32_t m_number_of_rva_and_sizes;
  data_directory_t m_data_directories;
};

}// namespace llama_debug


#endif// LLAMADEBUG_OPTIONAL_HEADER_H
