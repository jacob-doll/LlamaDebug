#ifndef LLAMADEBUG_PE_PROCESS_PARSER_H
#define LLAMADEBUG_PE_PROCESS_PARSER_H

#include <string>
#include <memory>

#include "llama_debug/binary/binary.h"
#include "llama_debug/process/process.h"

namespace llama_debug {

class pe_binary;
class pe_resource_directory;
class pe_resource_data_entry;

class pe_process_parser
{
public:
  static std::unique_ptr<binary> process_parse(
    process &proc,
    const std::string &name,
    uintptr_t base_addr);

private:
  pe_process_parser(process &proc,
    const std::string &name,
    uintptr_t base_addr);

  /* NT Header Parsion */
  uint32_t parse_dos_header();
  uint32_t parse_file_header(const uint32_t offset);
  uint32_t parse_optional_header(const uint32_t offset);
  void parse_sections(const uint32_t offset);

  void parse_exports();
  void parse_imports();
  void parse_resources();

  std::unique_ptr<pe_resource_directory> parse_resource_directory(
    const uintptr_t resource_dir_ptr,
    const uint32_t offset);
  std::unique_ptr<pe_resource_data_entry> parse_resource_data_entry(
    const uintptr_t resource_dir_ptr,
    const uint32_t offset);

private:
  process *m_proc;
  uintptr_t m_base_addr;
  pe_binary *m_binary;
};

}// namespace llama_debug

#endif// LLAMADEBUG_PE_PROCESS_PARSER_H
