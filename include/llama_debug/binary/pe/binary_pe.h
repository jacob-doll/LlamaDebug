#ifndef LLAMADEBUG_BINARYPE64_H
#define LLAMADEBUG_BINARYPE64_H

#include <llama_debug/binary/binary.h>
#include <llama_debug/binary/pe/defs.h>
#include <llama_debug/binary/pe/dos_header.h>
#include <llama_debug/binary/pe/file_header.h>
#include <llama_debug/binary/pe/optional_header.h>
#include <llama_debug/binary/pe/section_header.h>

#include <vector>

namespace llama_debug {

class binary_pe : public binary
{
public:
  using sections_t = std::vector<section_header>;

  binary_pe(const uint8_t *buffer, uint32_t size);

  static bool validate(const uint8_t *buffer, uint32_t size);

private:
  uint32_t rva_to_physical(uint32_t rva);
  bool from_buffer(const uint8_t *buffer, uint32_t size);
  uint32_t parse_headers(const uint8_t *buffer, uint32_t offset);
  void parse_sections(const uint8_t *buffer, uint32_t offset);
  void parse_imports(const uint8_t *buffer, uint32_t offset);

private:
  dos_header m_dos_header;
  uint32_t m_signature;
  file_header m_file_header;
  optional_header m_optional_header;  
  sections_t m_section_headers;
};

}// namespace llama_debug

#endif// LLAMADEBUG_BINARYPE64_H
