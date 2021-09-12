#include <iomanip>

#include "llama_debug/binary/pe/pe_binary.h"
#include "llama_debug/binary/pe/pe_resource_directory_entry.h"

namespace llama_debug {

pe_binary::pe_binary() = default;

pe_binary::pe_binary(const std::string &name)
  : binary{ name }
{}

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

uint32_t pe_binary::rva_to_physical(uint32_t rva)
{
  for (auto &sec : this->m_sections) {
    uint32_t section_virtual_address = sec->virtual_address();
    uint32_t section_virtual_size = sec->virtual_size();
    if (rva >= section_virtual_address && rva < section_virtual_address + section_virtual_size) {
      return sec->physical_address() + (rva - section_virtual_address);
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
  os << m_export_directory << "\n";

  os << std::setfill('-') << std::setw(96) << "\n";
  os << "IMPORTS\n";
  for (auto import_dir : m_import_directories) {
    os << std::setfill('-') << std::setw(96) << "\n";
    os << import_dir << "\n";
  }

  if (m_resource_root) {
    os << std::setfill('-') << std::setw(96) << "\n";
    os << "RESOURCES\n";
    os << *m_resource_root << "\n";
  }

  os.flags(old_settings);
  return os;
}

pe_dos_header &pe_binary::dos_header()
{
  return m_dos_header;
}

uint32_t pe_binary::signature()
{
  return m_signature;
}

pe_file_header &pe_binary::file_header()
{
  return m_file_header;
}

pe_optional_header &pe_binary::optional_header()
{
  return m_optional_header;
}

pe_export_directory &pe_binary::export_directory()
{
  return m_export_directory;
}

std::vector<pe_import_directory> &pe_binary::import_directories()
{
  return m_import_directories;
}

pe_resource_directory &pe_binary::resource_root()
{
  return *m_resource_root;
}

}// namespace llama_debug
