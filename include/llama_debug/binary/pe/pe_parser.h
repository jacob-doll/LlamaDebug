#ifndef LLAMADEBUG_PE_PARSER_H
#define LLAMADEBUG_PE_PARSER_H

#include <string>
#include <memory>

#include "llama_debug/binary/binary.h"

namespace llama_debug {
class pe_binary;
class pe_resource_directory;
class pe_resource_data_entry;

class pe_parser
{
public:
  static std::unique_ptr<binary> parse(const uint8_t *buffer, const uint32_t size);

private:
  pe_parser(const uint8_t *buffer, const uint32_t size);

  /* NT Header Parsion */
  uint32_t parse_dos_header();
  uint32_t parse_file_header(const uint32_t offset);
  uint32_t parse_optional_header(const uint32_t offset);
  void parse_sections(const uint32_t offset);

  void parse_exports();
  void parse_imports();
  void parse_resources();

  std::unique_ptr<pe_resource_directory> parse_resource_directory(const uint32_t resource_dir_ptr, const uint32_t offset);
  std::unique_ptr<pe_resource_data_entry> parse_resource_data_entry(const uint32_t resource_dir_ptr, const uint32_t offset);

private:
  const uint8_t *m_buffer;
  const uint32_t m_size;
  pe_binary *m_binary;
};

}// namespace llama_debug

#endif// LLAMADEBUG_PE_PARSER_H
