#ifndef LLAMADEBUG_WIN_PIPE_H
#define LLAMADEBUG_WIN_PIPE_H

#include "llama_debug/process/pipe.h"

namespace llama_debug {

class win_pipe : public pipe
{
public:
  typedef void *HANDLE;

  win_pipe();

  HANDLE read_handle();
  HANDLE write_handle();

  void close_read_handle();
  void close_write_handle();

  void disable_read();
  void disable_write();

  virtual const std::vector<uint8_t> read() override;
  virtual void write(const std::vector<uint8_t> &data) override;
  virtual void close() override;

private:
  void create_pipe();

private:
  HANDLE m_read_handle;
  HANDLE m_write_handle;
};

}// namespace llama_debug


#endif// LLAMADEBUG_WIN_PIPE_H