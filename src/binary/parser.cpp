#include "llama_debug/binary/parser.h"
#include "llama_debug/binary/pe/pe_parser.h"

namespace llama_debug {

static std::unique_ptr<binary> parse(const std::string &filename)
{
  if (pe_binary::validate())
    return nullptr;
}

static std::unique_ptr<binary> parse(const uint8_t *buffer, const uint32_t size)
{
  return nullptr;
}

}// namespace llama_debug
