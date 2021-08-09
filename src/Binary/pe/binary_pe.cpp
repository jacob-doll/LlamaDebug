#include <llama_debug/binary/pe/binary_pe.h>
#include <fstream>
#include <cstdio>

namespace llama_debug {

binary_pe::binary_pe(const uint8_t *buffer, uint32_t size)
{
  from_buffer(buffer, size);
}

binary_pe::~binary_pe()
{
  delete[] m_section_headers;
}

bool binary_pe::validate(const uint8_t *buffer, uint32_t size)
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
    // Check that executable is 64-Bit
    uint16_t optional_magic;
    std::memcpy(&optional_magic, buffer + exe_offset + 0x18, sizeof(optional_magic));
    if (optional_magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) return true;
  }
  return false;
}

uint32_t binary_pe::rva_to_physical(uint32_t rva)
{
  for (uint16_t i = 0; i < m_optional_header.NumberOfRvaAndSizes; i++) {
    uint32_t section_virtual_address = m_section_headers[i].VirtualAddress;
    uint32_t section_virtual_size = m_section_headers[i].Misc.VirtualSize;
    if (rva >= section_virtual_address && rva < section_virtual_address + section_virtual_size) {
      return m_section_headers[i].PointerToRawData + (rva - section_virtual_address);
    }
  }
  return rva;
}

bool binary_pe::from_buffer(const uint8_t *buffer, uint32_t size)
{
  if (!validate(buffer, size)) return false;

  uint32_t offset = 0;
  offset += parse_headers(buffer, offset);
  parse_sections(buffer, offset);

  const pe_image_data_directory import_directory = m_optional_header.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
  offset = rva_to_physical(import_directory.VirtualAddress);
  parse_imports(buffer, offset);

  return true;
}

uint32_t binary_pe::parse_headers(const uint8_t *buffer, uint32_t offset)
{
  const raw_dos_header* dos_header_ = (const raw_dos_header*) buffer;
  m_dos_header = dos_header{dos_header_};

  offset = m_dos_header.lfanew();
  std::memcpy(&m_signature, buffer + offset, sizeof(m_signature));
  offset += sizeof(m_signature);
  const raw_file_header* file_header_ = (const raw_file_header*)(buffer + offset);
  m_file_header = file_header{file_header_};
  offset += sizeof(raw_file_header);
  std::memcpy(&m_optional_header, buffer + offset, sizeof(m_optional_header));
  offset += sizeof(m_optional_header);

  m_entry_point = (uint64_t)m_optional_header.AddressOfEntryPoint + m_optional_header.ImageBase;
  m_base_addr = m_optional_header.ImageBase;
  return offset;
}

void binary_pe::parse_sections(const uint8_t *buffer, uint32_t offset)
{
  m_section_headers = new pe_image_section_header[m_file_header.number_of_sections()];

  for (uint16_t i = 0; i < m_file_header.number_of_sections(); i++) {
    std::memcpy(&m_section_headers[i], buffer + offset, sizeof(pe_image_section_header));
    m_sections.emplace_back(section{ std::string((char *)(m_section_headers[i].Name), IMAGE_SIZEOF_SHORT_NAME),
      m_section_headers[i].SizeOfRawData,
      m_section_headers[i].Misc.VirtualSize,
      m_section_headers[i].PointerToRawData,
      m_section_headers[i].VirtualAddress,
      0 });
    offset += sizeof(pe_image_section_header);
  }
}

void binary_pe::parse_imports(const uint8_t *buffer, uint32_t offset)
{
  while (true) {
    pe_image_import_directory *import_directory = (pe_image_import_directory *)(buffer + offset);
    if (!import_directory->OriginalFirstThunk) break;

    uint32_t name_offset = rva_to_physical(import_directory->Name);
    char *dll_name = (char *)(buffer + name_offset);

    uint32_t lookup_table_offset = rva_to_physical(import_directory->OriginalFirstThunk);
    uint32_t address_table_offset = rva_to_physical(import_directory->FirstThunk);

    while (true) {
      uint64_t image_thunk = *((uint64_t *)(buffer + lookup_table_offset));
      uint64_t address = *((uint64_t *)(buffer + address_table_offset));
      if (!image_thunk) break;
      if (!(image_thunk & 0x8000000000000000)) {
        name_offset = rva_to_physical((uint32_t)(image_thunk));
        pe_image_hint_name *hint_name = (pe_image_hint_name *)(buffer + name_offset);
        m_symbols.emplace_back(symbol{
          dll_name,
          hint_name->Name,
          address
        });
      }

      lookup_table_offset += sizeof(uint64_t);
      address_table_offset += sizeof(uint64_t);
    }

    offset += sizeof(pe_image_import_directory);
  }
}

}// namespace llama_debug
