#ifndef LLAMADEBUG_WIN_PROCESS_H
#define LLAMADEBUG_WIN_PROCESS_H

#include "llama_debug/process/process.h"

namespace llama_debug {

class win_process : public process
{
public:
  typedef void *HANDLE;

  win_process(const std::string &name, const std::string &args);

  virtual void close() override;
  virtual void kill(uint32_t exit_code) override;
  virtual bool is_active() override;
  virtual mapped_regions_t mapped_regions() override;
  virtual size_t read_memory(
    const uintptr_t addr,
    uint8_t *buffer,
    size_t size) override;

private:
  void init_process();

private:
  HANDLE m_proc_handle;
  HANDLE m_thread_handle;
  uint32_t m_tid;
};

}// namespace llama_debug


#endif//LLAMADEBUG_WIN_PROCESS_H
