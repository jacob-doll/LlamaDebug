#ifndef LLAMADEBUG_WIN_EXCEPTION_H
#define LLAMADEBUG_WIN_EXCEPTION_H

#include <exception>
#include <cstdint>

namespace llama_debug {

class win_exception : public std::exception
{
public:
  win_exception(uint32_t error);

private:
  virtual const char *what() const throw()
  {
    return msg;
  }
  char *msg;
};

}// namespace llama_debug

#endif// LLAMADEBUG_WIN_EXCEPTION_H
