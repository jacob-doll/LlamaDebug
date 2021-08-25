#include "llama_debug/binary/pe/pe_parser.h"
#include "llama_debug/binary/pe/pe_binary.h"

namespace llama_debug {

std::unique_ptr<binary> pe_parser::parse(const uint8_t *buffer, const uint32_t size)
{
  if (!pe_binary::validate(buffer, size)) {
    return nullptr;
  }

  pe_parser parser{ buffer, size };

  parser.parse_dos_header();

  uint32_t offset = parser.m_binary->m_dos_header.lfanew();
  parser.parse_file_header(offset);
  offset += sizeof(parser.m_binary->m_signature);
  offset += sizeof(raw_file_header);
  parser.parse_optional_header(offset);
  if (parser.m_binary->m_optional_header.magic() == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    offset += sizeof(raw_optional_header32);
  } else if (parser.m_binary->m_optional_header.magic() == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
    offset += sizeof(raw_optional_header64);
  }
  parser.parse_sections(offset);

  return std::unique_ptr<pe_binary>{ parser.m_binary };
}

pe_parser::pe_parser(const uint8_t *buffer, const uint32_t size)
  : m_buffer{ buffer },
    m_size{ size }
{
  m_binary = new pe_binary();
}

void pe_parser::parse_dos_header()
{
  const raw_dos_header *dos_header_ = (const raw_dos_header *)m_buffer;
  m_binary->m_dos_header = pe_dos_header{ dos_header_ };
}

void pe_parser::parse_file_header(const uint32_t offset)
{
  std::memcpy(&m_binary->m_signature, m_buffer + offset, sizeof(m_binary->m_signature));
  const raw_file_header *file_header_ = (const raw_file_header *)(m_buffer + offset + sizeof(m_binary->m_signature));
  m_binary->m_file_header = pe_file_header{ file_header_ };
}

void pe_parser::parse_optional_header(const uint32_t offset)
{
  uint16_t optional_magic;
  std::memcpy(&optional_magic, m_buffer + m_binary->m_dos_header.lfanew() + 0x18, sizeof(optional_magic));
  if (optional_magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    const raw_optional_header32 *optional_header_ = (const raw_optional_header32 *)(m_buffer + offset);
    m_binary->m_optional_header = pe_optional_header{ optional_header_ };
  } else if (optional_magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
    const raw_optional_header64 *optional_header_ = (const raw_optional_header64 *)(m_buffer + offset);
    m_binary->m_optional_header = pe_optional_header{ optional_header_ };
  }
}

void pe_parser::parse_sections(const uint32_t offset)
{
  uint32_t index = offset;
  for (uint16_t i = 0; i < m_binary->m_file_header.number_of_sections(); i++) {
    const raw_section_header *section_header_ = (const raw_section_header *)(m_buffer + index);
    m_binary->m_sections.emplace_back(
      std::make_unique<pe_section_header>(section_header_));
    index += sizeof(raw_section_header);
  }
}

void pe_parser::parse_exports()
{
}

void pe_parser::parse_imports()
{
}

void pe_parser::parse_resources()
{
}

}// namespace llama_debug
