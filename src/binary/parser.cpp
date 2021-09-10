#include "llama_debug/binary/parser.h"
#include "llama_debug/binary/mmap_file.h"
#include "llama_debug/binary/pe/pe_parser.h"
#include "llama_debug/binary/pe/pe_binary.h"

namespace llama_debug {

std::unique_ptr<binary> parse(const std::string &filename)
{
  mmap_file file{ filename };
  auto ret = parse(file.ptr(), file.size());
  return ret;
}

std::unique_ptr<binary> parse(const uint8_t *buffer, const uint32_t size)
{
  if (pe_binary::validate(buffer, size)) {
    return pe_parser::parse(buffer, size);
  }
  return nullptr;
}

std::unique_ptr<binary> process_parse(
  const process &proc,
  const std::string &name,
  uintptr_t base_addr)
{
  return nullptr;
}

}// namespace llama_debug
