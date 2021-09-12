#include "llama_debug/binary/pe/pe_process_parser.h"
#include "llama_debug/binary/pe/pe_binary.h"
#include "llama_debug/binary/pe/pe_resource_data_entry.h"

#include <cstdio>

namespace llama_debug {

std::unique_ptr<binary> pe_process_parser::process_parse(
  process &proc,
  const std::string &name,
  uintptr_t base_addr)
{
  pe_process_parser parser{ proc, name, base_addr };

  uint32_t offset = parser.parse_dos_header();
  offset += parser.parse_file_header(offset);
  offset += parser.parse_optional_header(offset);
  parser.parse_sections(offset);

  parser.parse_exports();
  parser.parse_imports();
  parser.parse_resources();

  return std::unique_ptr<pe_binary>{ parser.m_binary };
}

pe_process_parser::pe_process_parser(process &proc,
  const std::string &name,
  uintptr_t base_addr)
  : m_proc{ &proc },
    m_base_addr{ base_addr }
{
  m_binary = new pe_binary{ name };
}

/* NT Header Parsion */
uint32_t pe_process_parser::parse_dos_header()
{
  raw_dos_header dos_header_{};
  m_proc->read_memory(m_base_addr, dos_header_);
  m_binary->m_dos_header = pe_dos_header{ &dos_header_ };
  return m_binary->m_dos_header.lfanew();
}

uint32_t pe_process_parser::parse_file_header(const uint32_t offset)
{
  m_proc->read_memory(m_base_addr + offset, m_binary->m_signature);
  raw_file_header file_header_{};
  m_proc->read_memory(m_base_addr + offset + sizeof(m_binary->m_signature), file_header_);
  m_binary->m_file_header = pe_file_header{ &file_header_ };

  uint32_t bytes_read = sizeof(m_binary->m_signature);
  bytes_read += sizeof(raw_file_header);
  return bytes_read;
}

uint32_t pe_process_parser::parse_optional_header(const uint32_t offset)
{
  uint16_t optional_magic;
  m_proc->read_memory(m_base_addr + m_binary->m_dos_header.lfanew() + 0x18, optional_magic);
  if (optional_magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    raw_optional_header32 optional_header_{};
    m_proc->read_memory(m_base_addr + offset, optional_header_);
    m_binary->m_optional_header = pe_optional_header{ &optional_header_ };
    m_binary->m_base_addr = m_binary->m_optional_header.image_base();
    m_binary->m_entry_point = m_binary->m_optional_header.address_of_entry_point();
    return sizeof(raw_optional_header32);
  } else if (optional_magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
    raw_optional_header64 optional_header_{};
    m_proc->read_memory(m_base_addr + offset, optional_header_);
    m_binary->m_optional_header = pe_optional_header{ &optional_header_ };
    m_binary->m_base_addr = m_binary->m_optional_header.image_base();
    m_binary->m_entry_point = m_binary->m_optional_header.address_of_entry_point();
    return sizeof(raw_optional_header64);
  }
  return 0;
}

void pe_process_parser::parse_sections(const uint32_t offset)
{
  uint32_t index = offset;
  for (uint16_t i = 0; i < m_binary->m_file_header.number_of_sections(); i++) {
    raw_section_header section_header_{};
    m_proc->read_memory(m_base_addr + index, section_header_);
    m_binary->m_sections.emplace_back(
      std::make_shared<pe_section_header>(&section_header_));
    index += sizeof(raw_section_header);
  }
}

void pe_process_parser::parse_exports()
{
  const data_directory export_directory =
    m_binary->m_optional_header.data_directories().at(IMAGE_DIRECTORY_ENTRY_EXPORT);

  if (export_directory.size == 0) {
    return;
  }

  uintptr_t export_directory_addr = m_base_addr + export_directory.virtual_address;
  raw_export_directory export_directory_{};
  m_proc->read_memory(export_directory_addr, export_directory_);

  char dir_name[256];
  m_proc->read_memory(m_base_addr + export_directory_.name_rva, dir_name);
  puts(dir_name);
  m_binary->m_export_directory = pe_export_directory{ &export_directory_, std::string{ dir_name } };

  uintptr_t address_table_addr = m_base_addr + m_binary->m_export_directory.address_table_rva();
  uintptr_t name_table_addr = m_base_addr + m_binary->m_export_directory.name_pointer_rva();
  uintptr_t ordinal_table_addr = m_base_addr + m_binary->m_export_directory.ordinal_table_rva();

  uintptr_t address_table;
  m_proc->read_memory(address_table_addr, address_table);
  uintptr_t name_pointer_table;
  m_proc->read_memory(name_table_addr, name_pointer_table);
  uintptr_t ordinal_table;
  m_proc->read_memory(ordinal_table_addr, ordinal_table);

  for (uint32_t i = 0; i < m_binary->m_export_directory.number_of_names(); i++) {
    uint16_t ordinal;
    m_proc->read_memory(ordinal_table + i, ordinal);
    uint32_t name_rva;
    m_proc->read_memory(name_pointer_table + i, name_rva);

    uintptr_t name_addr = m_base_addr + name_rva;
    char name[256];
    m_proc->read_memory(name_addr, name);

    raw_export_address addr;
    m_proc->read_memory(address_table + i, addr);
    const data_directory export_directory =
      m_binary->m_optional_header.data_directories().at(IMAGE_DIRECTORY_ENTRY_EXPORT);
    std::string forwarder_name;
    if (addr.forwarder_rva < export_directory.virtual_address
        || addr.forwarder_rva >= export_directory.virtual_address + export_directory.size) {
      name_addr = m_base_addr + addr.forwarder_rva;
      char forwarder_name_[256];
      m_proc->read_memory(name_addr, forwarder_name_);
      forwarder_name = forwarder_name_;
    }

    auto entry = std::make_shared<pe_export_entry>(
      std::string{ dir_name },
      std::string{ name },
      addr.export_rva + m_binary->m_base_addr,
      addr,
      forwarder_name,
      name_rva,
      ordinal);

    m_binary->m_export_directory.add_export_entry(entry);
    m_binary->m_symbols.emplace_back(entry);
  }
}

void pe_process_parser::parse_imports()
{
  const data_directory import_directory =
    m_binary->m_optional_header.data_directories().at(IMAGE_DIRECTORY_ENTRY_IMPORT);

  if (import_directory.size == 0) {
    return;
  }

  uintptr_t import_directory_table_addr = m_base_addr + import_directory.virtual_address;

  while (true) {
    raw_import_directory import_directory_{};
    m_proc->read_memory(import_directory_table_addr, import_directory_);

    if (!import_directory_.import_lookup_table_rva) break;

    uintptr_t name_addr = m_base_addr + import_directory_.name_rva;
    char dir_name[256];
    m_proc->read_memory(name_addr, dir_name);

    pe_import_directory directory{ &import_directory_, std::string{ dir_name } };

    uintptr_t lookup_table_addr = m_base_addr + directory.import_lookup_table_rva();
    uintptr_t address_table_addr = m_base_addr + directory.import_address_table_rva();


    while (true) {
      uint64_t name_rva;
      m_proc->read_memory(lookup_table_addr, name_rva);
      uint64_t address;
      m_proc->read_memory(address_table_addr, address);

      if (!name_rva) break;
      if (!(name_rva & 0x8000000000000000)) {
        name_addr = m_base_addr + (uint32_t)(name_rva);
        uint16_t hint;
        m_proc->read_memory(name_addr, hint);
        char name[256];
        m_proc->read_memory(name_addr + sizeof(hint), name);

        auto entry = std::make_shared<pe_import_entry>(
          std::string{ dir_name },
          hint,
          std::string{ name },
          address,
          name_rva,
          0);

        directory.add_import_entry(entry);
        m_binary->m_symbols.emplace_back(entry);
      }

      lookup_table_addr += sizeof(uint64_t);
      address_table_addr += sizeof(uint64_t);
    }

    m_binary->m_import_directories.emplace_back(directory);
    import_directory_table_addr += sizeof(raw_import_directory);
  }
}

void pe_process_parser::parse_resources()
{
  const data_directory resource_directory =
    m_binary->m_optional_header.data_directories().at(IMAGE_DIRECTORY_ENTRY_RESOURCE);

  if (resource_directory.size == 0) {
    return;
  }

  const uintptr_t resource_dir_ptr = m_base_addr + resource_directory.virtual_address;
  m_binary->m_resource_root = parse_resource_directory(resource_dir_ptr, 0);
}

std::unique_ptr<pe_resource_directory> pe_process_parser::parse_resource_directory(
  const uintptr_t resource_dir_ptr,
  const uint32_t offset)
{
  uintptr_t index = resource_dir_ptr + offset;
  raw_resource_directory root_{};
  m_proc->read_memory(index, root_);
  std::unique_ptr<pe_resource_directory> ret = std::make_unique<pe_resource_directory>(&root_);

  uint16_t num_of_entries = ret->number_of_id_entries() + ret->number_of_named_entries();
  index += sizeof(raw_resource_directory);

  for (uint16_t i = 0; i < num_of_entries; i++) {
    raw_resource_directory_entry entry_{};
    m_proc->read_memory(index, entry_);
    pe_resource_directory_entry entry{ &entry_ };

    if (entry.is_directory_offset()) {
      uint32_t dir_offset = entry.offset_to_directory() & 0x7FFFFFFF;
      std::shared_ptr<pe_resource_directory> dir =
        this->parse_resource_directory(resource_dir_ptr, dir_offset);
      entry.directory(dir);
    } else {
      std::shared_ptr<pe_resource_data_entry> data =
        this->parse_resource_data_entry(resource_dir_ptr, entry.offset_to_data());
      entry.data_entry(data);
    }

    ret->add_entry(entry);
    index += sizeof(raw_resource_directory_entry);
  }

  return ret;
}

std::unique_ptr<pe_resource_data_entry> pe_process_parser::parse_resource_data_entry(
  const uintptr_t resource_dir_ptr,
  const uint32_t offset)
{
  uintptr_t index = resource_dir_ptr + offset;
  raw_resource_data_entry data_entry_{};
  m_proc->read_memory(index, data_entry_);
  return std::make_unique<pe_resource_data_entry>(&data_entry_);
}

}// namespace llama_debug
