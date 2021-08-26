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

  os << std::setfill('-') << std::setw(96) << "\n";
  os << "RESOURCES\n";
  os << *m_resource_root << "\n";

  os.flags(old_settings);
  return os;
}

}// namespace llama_debug
