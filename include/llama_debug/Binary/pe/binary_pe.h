#ifndef LLAMADEBUG_BINARYPE_H
#define LLAMADEBUG_BINARYPE_H

#include <llama_debug/binary/binary.h>
#include <llama_debug/binary/pe/defs.h>

namespace llama_debug {

class binary_pe : public binary
{
public:
  binary_pe(const uint8_t *buffer, uint32_t size);
  ~binary_pe();

  static bool validate(const uint8_t *buffer, uint32_t size);

  virtual binary_type type() override { return BINARY_PE; }

private:
  bool from_buffer(const uint8_t *buffer, uint32_t size);

private:
  PE_image_dos_header m_dos_headers;
  PE32_image_nt_headers m_headers;
  PE_image_section_header *m_section_headers;
};

}// namespace llama_debug

#endif// LLAMADEBUG_BINARYPE_H
