#include "llama_debug/binary/pe/pe_process_parser.h"
#include "llama_debug/binary/pe/pe_binary.h"
#include "llama_debug/binary/pe/pe_resource_data_entry.h"

namespace llama_debug {

std::unique_ptr<binary> pe_process_parser::process_parse(
  const process &proc,
  const std::string &name,
  uintptr_t base_addr)
{
  return nullptr;
}

pe_process_parser::pe_process_parser(const process &proc,
  const std::string &name,
  uintptr_t base_addr)
{}

/* NT Header Parsion */
uint32_t pe_process_parser::parse_dos_header()
{
  return 0;
}

uint32_t pe_process_parser::parse_file_header(const uint32_t offset)
{
  return 0;
}

uint32_t pe_process_parser::parse_optional_header(const uint32_t offset)
{
  return 0;
}

void pe_process_parser::parse_sections(const uint32_t offset)
{
}

void pe_process_parser::parse_exports()
{
}

void pe_process_parser::parse_imports()
{
}

void pe_process_parser::parse_resources()
{
}

std::unique_ptr<pe_resource_directory> pe_process_parser::parse_resource_directory(
  const uint32_t resource_dir_ptr,
  const uint32_t offset)
{
  return nullptr;
}

std::unique_ptr<pe_resource_data_entry> pe_process_parser::parse_resource_data_entry(
  const uint32_t resource_dir_ptr,
  const uint32_t offset)
{
  return nullptr;
}

}// namespace llama_debug
