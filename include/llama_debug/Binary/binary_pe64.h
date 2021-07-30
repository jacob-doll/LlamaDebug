#ifndef LLAMADEBUG_BINARYPE64_H
#define LLAMADEBUG_BINARYPE64_H

#include <llama_debug/binary/binary.h>
#include <llama_debug/binary/defs/PE.h>

namespace llama_debug {

class binary_pe64 : public binary
{
public:
  binary_pe64(const uint8_t *buffer, uint32_t size);
  ~binary_pe64();

  static bool validate(const uint8_t *buffer, uint32_t size);

  virtual binary_type type() override { return BINARY_PE64; }

private:
  bool from_buffer(const uint8_t *buffer, uint32_t size);
  uint32_t parse_headers(const uint8_t *buffer, uint32_t offset);
  void parse_sections(const uint8_t *buffer, uint32_t offset);
  void parse_imports(const uint8_t *buffer);

private:
  PEImageDosHeader m_dos_headers;
  PEImageNTHeaders64 m_headers;
  PEImageSectionHeader *m_section_headers;
};

}// namespace llama_debug

#endif// LLAMADEBUG_BINARYPE64_H
