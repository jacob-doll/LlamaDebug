#ifndef LLAMADEBUG_PE_BINARY_H
#define LLAMADEBUG_PE_BINARY_H

#include <vector>

#include "llama_debug/binary/binary.h"
#include "llama_debug/binary/pe/defs.h"
#include "llama_debug/binary/pe/pe_dos_header.h"
#include "llama_debug/binary/pe/pe_file_header.h"
#include "llama_debug/binary/pe/pe_import_directory.h"
#include "llama_debug/binary/pe/pe_optional_header.h"
#include "llama_debug/binary/pe/pe_section_header.h"

namespace llama_debug {

class pe_binary : public binary
{
public:
  using import_directories_t = std::vector<pe_import_directory>;

  pe_binary(const uint8_t *buffer, uint32_t size);

  static bool validate(const uint8_t *buffer, uint32_t size);

  virtual sections_t &sections() override;
  virtual std::ostream &print(std::ostream &os) const override;

private:
  uint32_t rva_to_physical(uint32_t rva);
  bool from_buffer(const uint8_t *buffer, uint32_t size);
  uint32_t parse_headers(const uint8_t *buffer, uint32_t offset);
  void parse_sections(const uint8_t *buffer, uint32_t offset);
  void parse_imports(const uint8_t *buffer, uint32_t offset);

private:
  pe_dos_header m_dos_header;
  uint32_t m_signature;
  pe_file_header m_file_header;
  pe_optional_header m_optional_header;
  sections_t m_sections;

  import_directories_t m_import_directories;
};

}// namespace llama_debug

#endif// LLAMADEBUG_PE_BINARY_H
