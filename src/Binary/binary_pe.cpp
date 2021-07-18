#include <llama_debug/binary/binary_pe.h>
#include <fstream>
#include <cstdio>

namespace llama_debug {

static uint32_t rva_to_physical(PEImageSectionHeader *sections, uint16_t num_sections, uint32_t rva)
{
  for (uint16_t i = 0; i < num_sections; i++) {
    uint32_t section_virtual_address = sections[i].VirtualAddress;
    uint32_t section_virtual_size = sections[i].Misc.VirtualSize;
    if (rva >= section_virtual_address && rva < section_virtual_address + section_virtual_size) {
      return sections[i].PointerToRawData + (rva - section_virtual_address);
    }
  }
  return rva;
}

binary_pe::~binary_pe()
{
  delete[] m_section_headers;
}

bool binary_pe::validate(const uint8_t *buffer, uint32_t size)
{
  // Must have the default MS-Dos header
  if (size < sizeof(PEImageDosHeader)) return false;
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
    // Check that executable is 32-Bit
    uint16_t optional_magic;
    std::memcpy(&optional_magic, buffer + exe_offset + 0x18, sizeof(optional_magic));
    if (optional_magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) return true;
  }
  return false;
}

bool binary_pe::from_file(const std::string &filename)
{
  // std::ifstream BinaryFile(Filename, std::ios::in|std::ios::binary);
  // if (!BinaryFile.is_open()) return false;

  // BinaryFile.read((char*)(&m_DosHeader), sizeof(m_DosHeader));
  // BinaryFile.seekg(m_DosHeader.e_lfanew, std::ios::beg);
  // BinaryFile.read((char*)(&m_Headers), sizeof(m_Headers));

  // m_SectionHeaders = new PEImageSectionHeader[m_Headers.FileHeader.NumberOfSections];

  // for (uint16_t i = 0; i < m_Headers.FileHeader.NumberOfSections; i++)
  // {
  //     BinaryFile.read((char*)(&m_SectionHeaders[i]), sizeof(PEImageSectionHeader));
  // }


  // BinaryFile.close();
  return true;
}

bool binary_pe::from_buffer(const uint8_t *buffer, uint32_t size)
{
  if (!validate(buffer, size)) return false;


  std::memcpy(&m_dos_headers, buffer, sizeof(m_dos_headers));
  uint32_t index = m_dos_headers.e_lfanew;
  std::memcpy(&m_headers, buffer + index, sizeof(m_headers));
  index += sizeof(m_headers);

  m_section_headers = new PEImageSectionHeader[m_headers.FileHeader.NumberOfSections];

  for (uint16_t i = 0; i < m_headers.FileHeader.NumberOfSections; i++) {
    std::memcpy(&m_section_headers[i], buffer + index, sizeof(PEImageSectionHeader));
    index += sizeof(PEImageSectionHeader);
  }

  return true;
}

void binary_pe::debug_print()
{
  for (uint16_t i = 0; i < m_headers.FileHeader.NumberOfSections; i++) {
    printf("%.8s\n", m_section_headers[i].Name);
  }
}

uintptr_t binary_pe::entry_point()
{
  return m_headers.OptionalHeader.AddressOfEntryPoint + m_headers.OptionalHeader.ImageBase;
}

}// namespace llama_debug
