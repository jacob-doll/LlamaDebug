#include <iomanip>

#include "llama_debug/binary/pe/pe_binary.h"
#include "llama_debug/binary/pe/pe_resource_directory_entry.h"

namespace llama_debug {

bool pe_binary::validate(const uint8_t *buffer, const uint32_t size)
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

// bool pe_binary::from_buffer(const uint8_t *buffer, uint32_t size)
// {


//   const data_directory resource_directory = m_optional_header.data_directories().at(IMAGE_DIRECTORY_ENTRY_RESOURCE);
//   offset = rva_to_physical(resource_directory.virtual_address);
//   parse_resources(buffer, offset);

//   return true;
// }

// void pe_binary::parse_resources(const uint8_t *buffer, const uint32_t offset)
// {
//   const data_directory resource_directory = m_optional_header.data_directories().at(IMAGE_DIRECTORY_ENTRY_RESOURCE);
//   const uint32_t resource_dir_ptr = rva_to_physical(resource_directory.virtual_address);

//   raw_resource_directory *root_ = (raw_resource_directory *)(buffer + offset);
//   m_resource_root = pe_resource_directory{ root_ };

//   uint16_t num_of_entries = m_resource_root.number_of_id_entries() + m_resource_root.number_of_named_entries();
//   uint32_t index = offset + sizeof(raw_resource_directory);
//   for (uint16_t i = 0; i < num_of_entries; i++) {
//     raw_resource_directory_entry *entry_ = (raw_resource_directory_entry *)(buffer + index);
//     pe_resource_directory_entry entry{ entry_ };
//     // if (entry.is_directory_offset()) {
//     //   uint32_t dir_offset = entry.offset_to_directory() & 0x7FFFFFFF;
//     //   this->parse_resources(buffer, resource_dir_ptr + dir_offset);
//     // }

//     m_resource_root.add_entry(pe_resource_directory_entry{ entry_ });
//     index += sizeof(raw_resource_directory_entry);
//   }
// }

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
  os << "EXPORTS\n";
  os << std::setfill('-') << std::setw(96) << "\n";
  os << m_export_directory.name() << "\n";
  for (auto export_entry : m_export_directory.export_entries()) {
    os << export_entry.name() << " | " << export_entry.address() << " | " << export_entry.forwarder_name() << "\n";
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

  // os << std::setfill('-') << std::setw(96) << "\n";
  // os << "RESOURCES\n";

  // // for (auto resource_dir : m_resource_directories) {
  // os << std::setfill('-') << std::setw(96) << "\n";
  // for (auto resource_entry : m_resource_root.entries()) {
  //   os << std::hex << resource_entry.id() << "\n";
  //   os << std::hex << resource_entry.offset_to_directory() << "\n";
  // }
  // // }

  os.flags(old_settings);
  return os;
}

}// namespace llama_debug
