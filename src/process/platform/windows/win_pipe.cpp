#include <windows.h>

#include "llama_debug/process/platform/windows/win_pipe.h"

namespace llama_debug {

win_pipe::win_pipe()
{
  create_pipe();
}

HANDLE win_pipe::read_handle()
{
  return m_read_handle;
}

HANDLE win_pipe::write_handle()
{
  return m_write_handle;
}

void win_pipe::close_read_handle()
{
  CloseHandle(m_read_handle);
}

void win_pipe::close_write_handle()
{
  CloseHandle(m_write_handle);
}

void win_pipe::disable_read()
{
  SetHandleInformation(m_read_handle, HANDLE_FLAG_INHERIT, 0);
}

void win_pipe::disable_write()
{
  SetHandleInformation(m_write_handle, HANDLE_FLAG_INHERIT, 0);
}

const std::vector<uint8_t> win_pipe::read()
{
  std::vector<uint8_t> ret(4096);
  DWORD dwRead;
  BOOL bSuccess = FALSE;
  uint32_t capacity = 4096;

  for (;;) {
    bSuccess = ReadFile(m_read_handle, &ret[0], ret.size(), &dwRead, nullptr);
    if (!bSuccess || dwRead == 0) break;
    capacity += 4096;
    ret.resize(capacity);
  }
  return ret;
}

void win_pipe::write(const std::vector<uint8_t> &data)
{
  DWORD dwWritten;
  if (!WriteFile(m_write_handle, data.data(), data.size(), &dwWritten, nullptr)) {
    return;
  }
}

void win_pipe::close()
{
  close_read_handle();
  close_write_handle();
}

void win_pipe::create_pipe()
{
  SECURITY_ATTRIBUTES saAttr;

  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  saAttr.lpSecurityDescriptor = NULL;

  if (!CreatePipe(&m_read_handle, &m_write_handle, &saAttr, 0)) {
    return;
  }
}


}// namespace llama_debug
