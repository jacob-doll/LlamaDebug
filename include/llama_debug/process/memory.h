#ifndef LLAMADEBUG_MEMORY_H
#define LLAMADEBUG_MEMORY_H

#include <string>
#include <cstdint>

namespace llama_debug {

struct mapped_region
{
  std::string mapped_file_name;
  uintptr_t base_addr;
  size_t region_size;
  uint32_t perms;
};

}// namespace llama_debug

#endif// LLAMADEBUG_MEMORY_H
