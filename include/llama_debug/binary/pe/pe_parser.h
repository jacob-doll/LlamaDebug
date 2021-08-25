#ifndef LLAMADEBUG_PE_PARSER_H
#define LLAMADEBUG_PE_PARSER_H

#include <string>
#include <memory>

#include "llama_debug/binary/binary.h"

namespace llama_debug {
class pe_binary;

class pe_parser
{
public:
  static std::unique_ptr<binary> parse(const uint8_t *buffer, const uint32_t size);

private:
  pe_parser(const uint8_t *buffer, const uint32_t size);

  /* NT Header Parsion */
  void parse_dos_header();
  void parse_file_header(const uint32_t offset);
  void parse_optional_header(const uint32_t offset);
  void parse_sections(const uint32_t offset);

  void parse_exports();
  void parse_imports();
  void parse_resources();

private:
  const uint8_t *m_buffer;
  const uint32_t m_size;
  pe_binary *m_binary;
};

}// namespace llama_debug

#endif// LLAMADEBUG_PE_PARSER_H
