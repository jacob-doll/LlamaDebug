#include <llama_debug/binary/binary_pe64.h>
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

binary_pe64::binary_pe64(const uint8_t *buffer, uint32_t size)
{
  from_buffer(buffer, size);
}

binary_pe64::~binary_pe64()
{
  delete[] m_section_headers;
}

bool binary_pe64::validate(const uint8_t *buffer, uint32_t size)
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
    // Check that executable is 64-Bit
    uint16_t optional_magic;
    std::memcpy(&optional_magic, buffer + exe_offset + 0x18, sizeof(optional_magic));
    if (optional_magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) return true;
  }
  return false;
}

bool binary_pe64::from_buffer(const uint8_t *buffer, uint32_t size)
{
  if (!validate(buffer, size)) return false;


  std::memcpy(&m_dos_headers, buffer, sizeof(m_dos_headers));
  uint32_t index = m_dos_headers.e_lfanew;
  std::memcpy(&m_headers, buffer + index, sizeof(m_headers));
  index += sizeof(m_headers);

  m_entry_point = (uint64_t)m_headers.OptionalHeader.AddressOfEntryPoint + m_headers.OptionalHeader.ImageBase;
  m_base_addr = m_headers.OptionalHeader.ImageBase;

  m_section_headers = new PEImageSectionHeader[m_headers.FileHeader.NumberOfSections];

  for (uint16_t i = 0; i < m_headers.FileHeader.NumberOfSections; i++) {
    std::memcpy(&m_section_headers[i], buffer + index, sizeof(PEImageSectionHeader));
    index += sizeof(PEImageSectionHeader);
  }

  return true;
}

void binary_pe64::debug_print()
{
  printf("Machine: %x\n", m_headers.FileHeader.Machine);
  printf("Entry point: %x\n", m_headers.OptionalHeader.AddressOfEntryPoint);
  printf("Base Addr: %llx\n", m_headers.OptionalHeader.ImageBase);
  for (uint16_t i = 0; i < m_headers.FileHeader.NumberOfSections; i++) {
    printf("%.8s\n", m_section_headers[i].Name);
  }
}

}// namespace llama_debug
