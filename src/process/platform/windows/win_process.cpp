#include <windows.h>
#include <exception>

#include "llama_debug/process/platform/windows/win_process.h"

namespace llama_debug {

class win_exception : public std::exception
{
public:
  win_exception(DWORD error)
  {
    FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      error,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPTSTR)&msg,
      0,
      NULL);
  }

private:
  virtual const char *what() const throw()
  {
    return msg;
  }
  char *msg;
};

win_process::win_process(const std::string &name, const std::string &args) : process(name, args)
{
  init_process();
}

void win_process::init_process()
{
  std::string command_str = m_name;
  command_str += " ";
  command_str += m_args;
  LPSTR command_line = const_cast<char *>(command_str.c_str());

  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  if (!CreateProcess(
        nullptr,
        command_line,
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        nullptr,
        &si,
        &pi)) {
    throw win_exception(GetLastError());
  }

  m_proc_handle = pi.hProcess;
  m_thread_handle = pi.hThread;
  m_pid = pi.dwProcessId;
  m_tid = pi.dwThreadId;
}

void win_process::close()
{
  CloseHandle(m_proc_handle);
  CloseHandle(m_thread_handle);
}

bool win_process::is_active()
{
  DWORD exit_code;
  GetExitCodeProcess(m_proc_handle, &exit_code);

  if (exit_code == STILL_ACTIVE) {
    return true;
  }
  return false;
}

}// namespace llama_debug
