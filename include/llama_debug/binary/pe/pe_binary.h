#ifndef LLAMADEBUG_PE_BINARY_H
#define LLAMADEBUG_PE_BINARY_H

#include <vector>

#include "llama_debug/binary/binary.h"
#include "llama_debug/binary/pe/defs.h"
#include "llama_debug/binary/pe/pe_dos_header.h"
#include "llama_debug/binary/pe/pe_file_header.h"
#include "llama_debug/binary/pe/pe_optional_header.h"
#include "llama_debug/binary/pe/pe_section_header.h"
#include "llama_debug/binary/pe/pe_export_directory.h"
#include "llama_debug/binary/pe/pe_import_directory.h"
#include "llama_debug/binary/pe/pe_resource_directory.h"

namespace llama_debug {
class pe_parser;

class pe_binary : public binary
{
  friend class pe_parser;

public:
  using import_directories_t = std::vector<pe_import_directory>;

  pe_binary();
  pe_binary(const std::string &name);

  static bool validate(const uint8_t *buffer, const uint32_t size);
  uint32_t rva_to_physical(uint32_t rva);

  virtual std::ostream &print(std::ostream &os) const override;

  pe_dos_header &dos_header();
  uint32_t signature();
  pe_file_header &file_header();
  pe_optional_header &optional_header();

  pe_export_directory &export_directory();
  import_directories_t &import_directories();
  pe_resource_directory &resource_root();

private:
  pe_dos_header m_dos_header;
  uint32_t m_signature;
  pe_file_header m_file_header;
  pe_optional_header m_optional_header;

  pe_export_directory m_export_directory;
  import_directories_t m_import_directories;
  std::unique_ptr<pe_resource_directory> m_resource_root;
};

}// namespace llama_debug

#endif// LLAMADEBUG_PE_BINARY_H
