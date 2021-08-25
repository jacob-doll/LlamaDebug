#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif /* _WIN32 */

#include "llama_debug/binary/mmap_file.h"

namespace llama_debug {

mmap_file::mmap_file(const std::string &filename)
{
  if (!init(filename)) {
    fprintf(stderr, "Couldn't create file\n");
  }
}

bool mmap_file::init(const std::string &filename)
{
#ifdef _WIN32
  m_file = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (m_file == INVALID_HANDLE_VALUE) {
    return false;
  }

  m_size = GetFileSize(m_file, NULL);

  m_map_file = CreateFileMapping(m_file, NULL, PAGE_READONLY, 0, 0, NULL);
  if (m_map_file == 0) {
    CloseHandle(m_file);
    return false;
  }

  m_ptr = (uint8_t *)MapViewOfFile(m_map_file, FILE_MAP_READ, 0, 0, 0);
  if (m_ptr == 0) {
    CloseHandle(m_file);
    CloseHandle(m_map_file);
    return false;
  }
  return true;
#endif
  return false;
}

void mmap_file::close()
{
#ifdef _WIN32
  UnmapViewOfFile((LPVOID)m_ptr);
  CloseHandle(m_map_file);
  CloseHandle(m_file);
#endif
}

}// namespace llama_debug
