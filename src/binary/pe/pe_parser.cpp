#include "llama_debug/binary/pe/pe_parser.h"
#include "llama_debug/binary/pe/pe_binary.h"
#include "llama_debug/binary/pe/pe_resource_data_entry.h"

namespace llama_debug {

std::unique_ptr<binary> pe_parser::parse(const uint8_t *buffer, const uint32_t size)
{
  if (!pe_binary::validate(buffer, size)) {
    return nullptr;
  }

  pe_parser parser{ buffer, size };

  uint32_t offset = parser.parse_dos_header();
  offset += parser.parse_file_header(offset);
  offset += parser.parse_optional_header(offset);
  parser.parse_sections(offset);

  parser.parse_exports();
  parser.parse_imports();
  parser.parse_resources();

  return std::unique_ptr<pe_binary>{ parser.m_binary };
}

pe_parser::pe_parser(const uint8_t *buffer, const uint32_t size)
  : m_buffer{ buffer },
    m_size{ size }
{
  m_binary = new pe_binary();
}

uint32_t pe_parser::parse_dos_header()
{
  const raw_dos_header *dos_header_ = (const raw_dos_header *)m_buffer;
  m_binary->m_dos_header = pe_dos_header{ dos_header_ };
  return m_binary->m_dos_header.lfanew();
}

uint32_t pe_parser::parse_file_header(const uint32_t offset)
{
  std::memcpy(&m_binary->m_signature, m_buffer + offset, sizeof(m_binary->m_signature));
  const raw_file_header *file_header_ = (const raw_file_header *)(m_buffer + offset + sizeof(m_binary->m_signature));
  m_binary->m_file_header = pe_file_header{ file_header_ };

  uint32_t bytes_read = sizeof(m_binary->m_signature);
  bytes_read += sizeof(raw_file_header);
  return bytes_read;
}

uint32_t pe_parser::parse_optional_header(const uint32_t offset)
{
  uint16_t optional_magic;
  std::memcpy(&optional_magic, m_buffer + m_binary->m_dos_header.lfanew() + 0x18, sizeof(optional_magic));
  if (optional_magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    const raw_optional_header32 *optional_header_ = (const raw_optional_header32 *)(m_buffer + offset);
    m_binary->m_optional_header = pe_optional_header{ optional_header_ };
    return sizeof(raw_optional_header32);
  } else if (optional_magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
    const raw_optional_header64 *optional_header_ = (const raw_optional_header64 *)(m_buffer + offset);
    m_binary->m_optional_header = pe_optional_header{ optional_header_ };
    return sizeof(raw_optional_header64);
  }
  return 0;
}

void pe_parser::parse_sections(const uint32_t offset)
{
  uint32_t index = offset;
  for (uint16_t i = 0; i < m_binary->m_file_header.number_of_sections(); i++) {
    const raw_section_header *section_header_ = (const raw_section_header *)(m_buffer + index);
    m_binary->m_sections.emplace_back(
      std::make_shared<pe_section_header>(section_header_));
    index += sizeof(raw_section_header);
  }
}

void pe_parser::parse_exports()
{
  const data_directory export_directory = m_binary->m_optional_header.data_directories().at(IMAGE_DIRECTORY_ENTRY_EXPORT);
  uint32_t offset = m_binary->rva_to_physical(export_directory.virtual_address);

  raw_export_directory *export_directory_ = (raw_export_directory *)(m_buffer + offset);
  uint32_t name_offset = m_binary->rva_to_physical(export_directory_->name_rva);

  char *dir_name = (char *)(m_buffer + name_offset);

  m_binary->m_export_directory = { export_directory_, std::string{ dir_name } };

  uint32_t address_table_offset = m_binary->rva_to_physical(m_binary->m_export_directory.address_table_rva());
  uint32_t name_table_offset = m_binary->rva_to_physical(m_binary->m_export_directory.name_pointer_rva());
  uint32_t ordinal_table_offset = m_binary->rva_to_physical(m_binary->m_export_directory.ordinal_table_rva());

  raw_export_address *address_table = (raw_export_address *)(m_buffer + address_table_offset);
  uint32_t *name_pointer_table = (uint32_t *)(m_buffer + name_table_offset);
  uint16_t *ordinal_table = (uint16_t *)(m_buffer + ordinal_table_offset);

  for (uint32_t i = 0; i < m_binary->m_export_directory.number_of_names(); i++) {
    uint16_t ordinal = ordinal_table[i];
    uint32_t name_rva = name_pointer_table[i];

    uint32_t name_offset = m_binary->rva_to_physical(name_rva);
    char *name = (char *)(m_buffer + name_offset);

    raw_export_address addr = address_table[i];
    const data_directory export_directory = m_binary->m_optional_header.data_directories().at(IMAGE_DIRECTORY_ENTRY_EXPORT);
    if (addr.forwarder_rva < export_directory.virtual_address
        || addr.forwarder_rva >= export_directory.virtual_address + export_directory.size) {
      name_offset = m_binary->rva_to_physical(addr.forwarder_rva);
      char *forwarder_name = (char *)(m_buffer + name_offset);
    }

    auto entry = std::make_shared<pe_export_entry>(
      std::string{ dir_name },
      std::string{ name },
      addr.export_rva + m_binary->m_base_addr,
      addr,
      std::string{},
      name_rva,
      ordinal);

    m_binary->m_export_directory.add_export_entry(entry);
    m_binary->m_symbols.emplace_back(entry);
  }
}

void pe_parser::parse_imports()
{
  const data_directory import_directory = m_binary->m_optional_header.data_directories().at(IMAGE_DIRECTORY_ENTRY_IMPORT);
  uint32_t offset = m_binary->rva_to_physical(import_directory.virtual_address);

  while (true) {
    raw_import_directory *import_directory_ = (raw_import_directory *)(m_buffer + offset);
    if (!import_directory_->import_lookup_table_rva) break;

    uint32_t name_offset = m_binary->rva_to_physical(import_directory_->name_rva);
    char *dir_name = (char *)(m_buffer + name_offset);

    pe_import_directory directory{ import_directory_, std::string{ dir_name } };

    uint32_t lookup_table_offset = m_binary->rva_to_physical(directory.import_lookup_table_rva());
    uint32_t address_table_offset = m_binary->rva_to_physical(directory.import_address_table_rva());


    while (true) {
      uint64_t name_rva = *((uint64_t *)(m_buffer + lookup_table_offset));
      uint64_t address = *((uint64_t *)(m_buffer + address_table_offset));
      if (!name_rva) break;
      if (!(name_rva & 0x8000000000000000)) {
        name_offset = m_binary->rva_to_physical((uint32_t)(name_rva));
        raw_hint_name *hint_name_ = (raw_hint_name *)(m_buffer + name_offset);

        auto entry = std::make_shared<pe_import_entry>(
          std::string{ dir_name },
          hint_name_,
          address,
          name_rva,
          0);

        directory.add_import_entry(entry);
        m_binary->m_symbols.emplace_back(entry);
      }

      lookup_table_offset += sizeof(uint64_t);
      address_table_offset += sizeof(uint64_t);
    }

    m_binary->m_import_directories.emplace_back(directory);
    offset += sizeof(raw_import_directory);
  }
}

void pe_parser::parse_resources()
{
  const data_directory resource_directory = m_binary->m_optional_header.data_directories().at(IMAGE_DIRECTORY_ENTRY_RESOURCE);
  const uint32_t resource_dir_ptr = m_binary->rva_to_physical(resource_directory.virtual_address);
  m_binary->m_resource_root = parse_resource_directory(resource_dir_ptr, 0);
}

std::unique_ptr<pe_resource_directory> pe_parser::parse_resource_directory(const uint32_t resource_dir_ptr, const uint32_t offset)
{
  uint32_t index = resource_dir_ptr + offset;
  raw_resource_directory *root_ = (raw_resource_directory *)(m_buffer + index);
  std::unique_ptr<pe_resource_directory> ret = std::make_unique<pe_resource_directory>(root_);

  uint16_t num_of_entries = ret->number_of_id_entries() + ret->number_of_named_entries();
  index += sizeof(raw_resource_directory);
  for (uint16_t i = 0; i < num_of_entries; i++) {
    raw_resource_directory_entry *entry_ = (raw_resource_directory_entry *)(m_buffer + index);
    pe_resource_directory_entry entry{ entry_ };

    if (entry.is_directory_offset()) {
      uint32_t dir_offset = entry.offset_to_directory() & 0x7FFFFFFF;
      std::shared_ptr<pe_resource_directory> dir = this->parse_resource_directory(resource_dir_ptr, dir_offset);
      entry.directory(dir);
    } else {
      std::shared_ptr<pe_resource_data_entry> data = this->parse_resource_data_entry(resource_dir_ptr, entry.offset_to_data());
      entry.data_entry(data);
    }

    ret->add_entry(entry);
    index += sizeof(raw_resource_directory_entry);
  }
  return ret;
}

std::unique_ptr<pe_resource_data_entry> pe_parser::parse_resource_data_entry(const uint32_t resource_dir_ptr, const uint32_t offset)
{
  uint32_t index = resource_dir_ptr + offset;
  raw_resource_data_entry *data_entry_ = (raw_resource_data_entry *)(m_buffer + index);
  return std::make_unique<pe_resource_data_entry>(data_entry_);
}


}// namespace llama_debug
