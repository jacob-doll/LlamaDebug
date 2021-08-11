#ifndef LLAMADEBUG_SECTION_H
#define LLAMADEBUG_SECTION_H

#include <cstdint>
#include <string>

namespace llama_debug
{

class section
{
public:
  virtual std::string name() const = 0;
  virtual uint32_t virtual_size() const = 0;
  virtual uintptr_t virtual_address() const = 0;
  virtual uint32_t physical_size() const = 0;
  virtual uintptr_t physical_address() const = 0;
};
  
} // namespace llama_debug


#endif // LLAMADEBUG_SECTION_H
