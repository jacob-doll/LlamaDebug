#include "llama_debug/binary/pe/optional_header.h"
#include "llama_debug/binary/pe/defs.h"

namespace llama_debug {

optional_header::optional_header()
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
    m_dll_characteristics{ 0x00000000 },
    m_size_of_stack_reserve{ 0x0000000000000000 },
    m_size_of_stack_commit{ 0x0000000000000000 },
    m_size_of_heap_reserve{ 0x0000000000000000 },
    m_size_of_heap_commit{ 0x0000000000000000 },
    m_loader_flags{ 0x00000000 },
    m_number_of_rva_and_sizes{ 16 },
    m_data_directories{}
{}

optional_header::optional_header(const raw_optional_header32 *data)
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
    m_dll_characteristics{ data->dll_characteristics },
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
}

optional_header::optional_header(const raw_optional_header64 *data)
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
    m_dll_characteristics{ data->dll_characteristics },
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
}

uint16_t optional_header::magic() const
{
  return m_magic;
}

uint8_t optional_header::major_linker_version() const
{
  return m_major_linker_version;
}

uint8_t optional_header::minor_linker_version() const
{
  return m_minor_linker_version;
}

uint32_t optional_header::size_of_code() const
{
  return m_size_of_code;
}

uint32_t optional_header::size_of_initialized_data() const
{
  return m_size_of_initialized_data;
}

uint32_t optional_header::size_of_uninitialized_data() const
{
  return m_size_of_uninitialized_data;
}

uint32_t optional_header::address_of_entry_point() const
{
  return m_address_of_entry_point;
}

uint32_t optional_header::base_of_code() const
{
  return m_base_of_code;
}

uint32_t optional_header::base_of_data() const
{
  return m_base_of_data;
}

uint64_t optional_header::image_base() const
{
  return m_image_base;
}

uint32_t optional_header::section_alignment() const
{
  return m_section_alignment;
}

uint32_t optional_header::file_alignment() const
{
  return m_file_alignment;
}

uint16_t optional_header::major_operating_system_version() const
{
  return m_major_operating_system_version;
}

uint16_t optional_header::minor_operating_system_version() const
{
  return m_minor_operating_system_version;
}

uint16_t optional_header::major_image_version() const
{
  return m_major_image_version;
}

uint16_t optional_header::minor_image_version() const
{
  return m_minor_image_version;
}

uint16_t optional_header::major_subsystem_version() const
{
  return m_major_subsystem_version;
}

uint16_t optional_header::minor_subsystem_version() const
{
  return m_minor_subsystem_version;
}

uint32_t optional_header::win32_version_value() const
{
  return m_win32_version_value;
}

uint32_t optional_header::size_of_image() const
{
  return m_size_of_image;
}

uint32_t optional_header::size_of_headers() const
{
  return m_size_of_headers;
}

uint32_t optional_header::checksum() const
{
  return m_checksum;
}

uint16_t optional_header::subsystem() const
{
  return m_subsystem;
}

uint16_t optional_header::dll_characteristics() const
{
  return m_dll_characteristics;
}

uint64_t optional_header::size_of_stack_reserve() const
{
  return m_size_of_stack_reserve;
}

uint64_t optional_header::size_of_stack_commit() const
{
  return m_size_of_stack_commit;
}

uint64_t optional_header::size_of_heap_reserve() const
{
  return m_size_of_heap_reserve;
}

uint64_t optional_header::size_of_heap_commit() const
{
  return m_size_of_heap_commit;
}

uint32_t optional_header::loader_flags() const
{
  return m_loader_flags;
}

uint32_t optional_header::number_of_rva_and_sizes() const
{
  return m_number_of_rva_and_sizes;
}

std::array<data_directory, IMAGE_NUMBEROF_DIRECTORY_ENTRIES> optional_header::data_directories() const
{
  return m_data_directories;
}

void optional_header::magic(const uint16_t magic)
{
  m_magic = magic;
}

void optional_header::major_linker_version(const uint8_t major_linker_version)
{
  m_major_linker_version = major_linker_version;
}

void optional_header::minor_linker_version(const uint8_t minor_linker_version)
{
  m_minor_linker_version = minor_linker_version;
}

void optional_header::size_of_code(const uint32_t size_of_code)
{
  m_size_of_code = size_of_code;
}

void optional_header::size_of_initialized_data(const uint32_t size_of_initialized_data)
{
  m_size_of_initialized_data = size_of_initialized_data;
}

void optional_header::size_of_uninitialized_data(const uint32_t size_of_uninitialized_data)
{
  m_size_of_uninitialized_data = size_of_uninitialized_data;
}

void optional_header::address_of_entry_point(const uint32_t address_of_entry_point)
{
  m_address_of_entry_point = address_of_entry_point;
}

void optional_header::base_of_code(const uint32_t base_of_code)
{
  m_base_of_code = base_of_code;
}

void optional_header::base_of_data(const uint32_t base_of_data)
{
  m_base_of_data = base_of_data;
}

void optional_header::image_base(const uint64_t image_base)
{
  m_image_base = image_base;
}

void optional_header::section_alignment(const uint32_t section_alignment)
{
  m_section_alignment = section_alignment;
}

void optional_header::file_alignment(const uint32_t file_alignment)
{
  m_file_alignment = file_alignment;
}

void optional_header::major_operating_system_version(const uint16_t major_operating_system_version)
{
  m_major_operating_system_version = major_operating_system_version;
}

void optional_header::minor_operating_system_version(const uint16_t minor_operating_system_version)
{
  m_minor_operating_system_version = minor_operating_system_version;
}

void optional_header::major_image_version(const uint16_t major_image_version)
{
  m_major_image_version = major_image_version;
}

void optional_header::minor_image_version(const uint16_t minor_image_version)
{
  m_minor_image_version = minor_image_version;
}

void optional_header::major_subsystem_version(const uint16_t major_subsystem_version)
{
  m_major_subsystem_version = major_subsystem_version;
}

void optional_header::minor_subsystem_version(const uint16_t minor_subsystem_version)
{
  m_minor_subsystem_version = minor_subsystem_version;
}

void optional_header::win32_version_value(const uint32_t win32_version_value)
{
  m_win32_version_value = win32_version_value;
}

void optional_header::size_of_image(const uint32_t size_of_image)
{
  m_size_of_image = size_of_image;
}

void optional_header::size_of_headers(const uint32_t size_of_headers)
{
  m_size_of_headers = size_of_headers;
}

void optional_header::checksum(const uint32_t checksum)
{
  m_checksum = checksum;
}

void optional_header::subsystem(const uint16_t subsystem)
{
  m_subsystem = subsystem;
}

void optional_header::dll_characteristics(const uint16_t dll_characteristics)
{
  m_dll_characteristics = dll_characteristics;
}

void optional_header::size_of_stack_reserve(const uint64_t size_of_stack_reserve)
{
  m_size_of_stack_reserve = size_of_stack_reserve;
}

void optional_header::size_of_stack_commit(const uint64_t size_of_stack_commit)
{
  m_size_of_stack_commit = size_of_stack_commit;
}

void optional_header::size_of_heap_reserve(const uint64_t size_of_heap_reserve)
{
  m_size_of_heap_reserve = size_of_heap_reserve;
}

void optional_header::size_of_heap_commit(const uint64_t size_of_heap_commit)
{
  m_size_of_heap_commit = size_of_heap_commit;
}

void optional_header::loader_flags(const uint32_t loader_flags)
{
  m_loader_flags = loader_flags;
}

void optional_header::number_of_rva_and_sizes(const uint32_t number_of_rva_and_sizes)
{
  m_number_of_rva_and_sizes = number_of_rva_and_sizes;
}

void optional_header::data_directories(const std::array<data_directory, IMAGE_NUMBEROF_DIRECTORY_ENTRIES> &data_directories)
{
  m_data_directories = data_directories;
}


}// namespace llama_debug
