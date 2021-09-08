#include <windows.h>

#include "llama_debug/process/platform/windows/win_process.h"
#include "llama_debug/process/platform/windows/win_pipe.h"
#include "llama_debug/process/platform/windows/win_exception.h"

namespace llama_debug {

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

  win_pipe *std_out_ = dynamic_cast<win_pipe *>(m_std_out.get());
  std_out_->disable_read();
  win_pipe *std_in_ = dynamic_cast<win_pipe *>(m_std_in.get());
  std_in_->disable_write();

  PROCESS_INFORMATION pi;
  STARTUPINFO si;

  ZeroMemory(&pi, sizeof(pi));

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  si.hStdError = std_out_->write_handle();
  si.hStdOutput = std_out_->write_handle();
  si.hStdInput = std_in_->read_handle();
  si.dwFlags |= STARTF_USESTDHANDLES;

  if (!CreateProcess(
        nullptr,
        command_line,
        nullptr,
        nullptr,
        TRUE,
        0,
        nullptr,
        nullptr,
        &si,
        &pi)) {
    throw win_exception(GetLastError());
  }

  std_out_->close_write_handle();
  std_in_->close_read_handle();

  m_proc_handle = pi.hProcess;
  m_thread_handle = pi.hThread;
  m_pid = pi.dwProcessId;
  m_tid = pi.dwThreadId;
}

void win_process::close()
{
  if (!this->is_active()) {
    this->kill(9);
  }
  CloseHandle(m_proc_handle);
  CloseHandle(m_thread_handle);
}

void win_process::kill(uint32_t exit_code)
{
  TerminateProcess(m_proc_handle, exit_code);
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
