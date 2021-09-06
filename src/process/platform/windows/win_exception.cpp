#include <windows.h>

#include "llama_debug/process/platform/windows/win_exception.h"

namespace llama_debug {

win_exception::win_exception(uint32_t error)
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

}// namespace llama_debug
