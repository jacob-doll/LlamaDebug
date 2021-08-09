#include <llama_debug/binary/binary.h>
#include <llama_debug/binary/pe/binary_pe.h>
#include <llama_debug/binary/mmap_file.h>

namespace llama_debug {

// here to change filename

binary *binary::from_file(const std::string &filename)
{
  mmap_file file(filename);
  binary *ret = from_buffer(file.ptr(), file.size());
  file.close();
  return ret;
}

binary *binary::from_buffer(const uint8_t *buffer, uint32_t size)
{
  binary *ret = nullptr;
  if (binary_pe::validate(buffer, size)) {
    ret = new binary_pe(buffer, size);
  }
  return ret;
}

}// namespace llama_debug
