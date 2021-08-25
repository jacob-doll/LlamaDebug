#ifndef LLAMADEBUG_PE_PARSER_H
#define LLAMADEBUG_PE_PARSER_H

#include <string>
#include <memory>

#include "llama_debug/binary/binary.h"
#include "llama_debug/binary/pe/pe_binary.h"

namespace llama_debug {

class pe_parser
{
public:
  static std::unique_ptr<binary> parse(const std::string &filename);
  static std::unique_ptr<binary> parse(const uint8_t *buffer, const uint32_t size);
};

}// namespace llama_debug

#endif// LLAMADEBUG_PE_PARSER_H
