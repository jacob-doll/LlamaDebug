#include <iomanip>

#include "llama_debug/binary/pe/pe_binary.h"

namespace llama_debug {

pe_binary::pe_binary(const uint8_t *buffer, uint32_t size)
{
  from_buffer(buffer, size);
}

bool pe_binary::validate(const uint8_t *buffer, uint32_t size)
{
  // Must have the default MS-Dos header
  if (size < sizeof(raw_dos_header)) return false;
  // Check Dos Signature
  uint16_t dos_signature;
  std::memcpy(&dos_signature, buffer, sizeof(dos_signature));
  if (dos_signature == IMAGE_DOS_SIGNATURE) {
    uint32_t exe_offset;
    std::memcpy(&exe_offset, buffer + 0x3c, sizeof(exe_offset));
    // Check NT Signature 0x00004550 (PE)
    uint32_t nt_signature;
    std::memcpy(&nt_signature, buffer + exe_offset, sizeof(nt_signature));
    if (nt_signature != IMAGE_NT_SIGNATURE) return false;
    return true;
  }
  return false;
}

sections_t &pe_binary::sections()
{
  return m_sections;
}

uint32_t pe_binary::rva_to_physical(uint32_t rva)
{
  for (uint16_t i = 0; i < m_optional_header.number_of_rva_and_sizes(); i++) {
    uint32_t section_virtual_address = m_sections.at(i)->virtual_address();
    uint32_t section_virtual_size = m_sections.at(i)->virtual_size();
    if (rva >= section_virtual_address && rva < section_virtual_address + section_virtual_size) {
      return m_sections.at(i)->physical_address() + (rva - section_virtual_address);
    }
  }
  return rva;
}

bool pe_binary::from_buffer(const uint8_t *buffer, uint32_t size)
{
  if (!validate(buffer, size)) return false;

  uint32_t offset = 0;
  offset += parse_headers(buffer, offset);
  parse_sections(buffer, offset);

  const data_directory import_directory = m_optional_header.data_directories().at(IMAGE_DIRECTORY_ENTRY_IMPORT);
  offset = rva_to_physical(import_directory.virtual_address);
  parse_imports(buffer, offset);

  return true;
}

uint32_t pe_binary::parse_headers(const uint8_t *buffer, uint32_t offset)
{
  const raw_dos_header *dos_header_ = (const raw_dos_header *)buffer;
  m_dos_header = pe_dos_header{ dos_header_ };

  offset = m_dos_header.lfanew();

  std::memcpy(&m_signature, buffer + offset, sizeof(m_signature));
  offset += sizeof(m_signature);

  const raw_file_header *file_header_ = (const raw_file_header *)(buffer + offset);
  m_file_header = pe_file_header{ file_header_ };
  offset += sizeof(raw_file_header);

  uint16_t optional_magic;
  std::memcpy(&optional_magic, buffer + m_dos_header.lfanew() + 0x18, sizeof(optional_magic));
  if (optional_magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    const raw_optional_header32 *optional_header_ = (const raw_optional_header32 *)(buffer + offset);
    m_optional_header = pe_optional_header{ optional_header_ };
    offset += sizeof(raw_optional_header32);
  } else if (optional_magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
    const raw_optional_header64 *optional_header_ = (const raw_optional_header64 *)(buffer + offset);
    m_optional_header = pe_optional_header{ optional_header_ };
    offset += sizeof(raw_optional_header64);
  }

  m_entry_point = (uint64_t)m_optional_header.address_of_entry_point() + m_optional_header.image_base();
  m_base_addr = m_optional_header.image_base();
  return offset;
}

void pe_binary::parse_sections(const uint8_t *buffer, uint32_t offset)
{
  for (uint16_t i = 0; i < m_file_header.number_of_sections(); i++) {
    const raw_section_header *section_header_ = (const raw_section_header *)(buffer + offset);
    m_sections.emplace_back(
      std::make_unique<pe_section_header>(section_header_));
    offset += sizeof(raw_section_header);
  }
}

void pe_binary::parse_imports(const uint8_t *buffer, uint32_t offset)
{
  while (true) {
    raw_import_directory *import_directory_ = (raw_import_directory *)(buffer + offset);
    if (!import_directory_->import_lookup_table_rva) break;

    uint32_t name_offset = rva_to_physical(import_directory_->name_rva);
    char *dll_name = (char *)(buffer + name_offset);

    pe_import_directory directory{ import_directory_, std::string{ dll_name } };

    uint32_t lookup_table_offset = rva_to_physical(directory.import_lookup_table_rva());
    uint32_t address_table_offset = rva_to_physical(directory.import_address_table_rva());

    while (true) {
      uint64_t name_rva = *((uint64_t *)(buffer + lookup_table_offset));
      uint64_t address = *((uint64_t *)(buffer + address_table_offset));
      if (!name_rva) break;
      if (!(name_rva & 0x8000000000000000)) {
        name_offset = rva_to_physical((uint32_t)(name_rva));
        raw_hint_name *hint_name_ = (raw_hint_name *)(buffer + name_offset);
        directory.add_import_entry(pe_import_entry{
          std::string{ dll_name },
          hint_name_,
          address,
          name_rva,
          0 });
      }

      lookup_table_offset += sizeof(uint64_t);
      address_table_offset += sizeof(uint64_t);
    }

    m_import_directories.emplace_back(directory);
    offset += sizeof(raw_import_directory);
  }
}

std::ostream &pe_binary::print(std::ostream &os) const
{
  std::ios::fmtflags old_settings = os.flags();

  switch (m_optional_header.magic()) {
  case IMAGE_NT_OPTIONAL_HDR32_MAGIC:
    os << "PE32\n";
    break;
  case IMAGE_NT_OPTIONAL_HDR64_MAGIC:
    os << "PE32+\n";
    break;
  }

  os << std::setfill('-') << std::setw(96) << "\n";
  os << "DOS Header:\n";
  os << m_dos_header << "\n";

  os << std::setfill('-') << std::setw(96) << "\n";
  os << "Signature: " << std::hex << m_signature << "\n";

  os << std::setfill('-') << std::setw(96) << "\n";
  os << "File Header:\n";
  os << m_file_header << "\n";

  os << std::setfill('-') << std::setw(96) << "\n";
  os << "Optional Header:\n";
  os << m_optional_header << "\n";

  os << std::setfill('-') << std::setw(96) << "\n";
  os << "SECTIONS\n";

  for (auto &section : m_sections) {
    os << std::setfill('-') << std::setw(96) << "\n";
    os << (*section) << "\n";
  }

  os << std::setfill('-') << std::setw(96) << "\n";
  os << "IMPORTS\n";

  for (auto import_dir : m_import_directories) {
    os << std::setfill('-') << std::setw(96) << "\n";
    os << import_dir.name() << "\n";
    for (auto import_entry : import_dir.import_entries()) {
      os << import_entry.name() << "\n";
    }
  }

  os.flags(old_settings);
  return os;
}

}// namespace llama_debug
