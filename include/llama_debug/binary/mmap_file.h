#ifndef LLAMADEBUG_MMAP_FILE_H
#define LLAMADEBUG_MMAP_FILE_H

#include <string>

namespace llama_debug {

class mmap_file
{
public:
  mmap_file(const std::string &filename);

  void close();

  uint8_t *ptr() { return m_ptr; }
  uint32_t size() { return m_size; }

private:
  bool init(const std::string &filename);

private:
#ifdef _WIN32
  typedef void *HANDLE;
  HANDLE m_file;
  HANDLE m_map_file;
#endif
  uint8_t *m_ptr;
  uint32_t m_size;
};

}// namespace llama_debug

#endif// LLAMADEBUG_MMAP_FILE_H
