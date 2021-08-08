#ifndef LLAMADEBUG_BINARYPE64_H
#define LLAMADEBUG_BINARYPE64_H

#include <llama_debug/binary/binary.h>
#include <llama_debug/binary/pe/defs.h>

namespace llama_debug {

class binary_pe64 : public binary
{
public:
  binary_pe64(const uint8_t *buffer, uint32_t size);
  ~binary_pe64();

  static bool validate(const uint8_t *buffer, uint32_t size);

  virtual binary_type type() override { return BINARY_PE64; }

private:
  uint32_t rva_to_physical(uint32_t rva);
  bool from_buffer(const uint8_t *buffer, uint32_t size);
  uint32_t parse_headers(const uint8_t *buffer, uint32_t offset);
  void parse_sections(const uint8_t *buffer, uint32_t offset);
  void parse_imports(const uint8_t *buffer, uint32_t offset);

private:
  PE_image_dos_header m_dos_headers;
  PE64_image_nt_headers m_headers;
  PE_image_section_header *m_section_headers;
};

}// namespace llama_debug

#endif// LLAMADEBUG_BINARYPE64_H
