#ifndef LLAMADEBUG_WIN_PROCESS_H
#define LLAMADEBUG_WIN_PROCESS_H

#include "llama_debug/process/process.h"

namespace llama_debug {

class win_process : public process
{
public:
  win_process(const std::string &name, const std::string &args);
};

}// namespace llama_debug


#endif//LLAMADEBUG_WIN_PROCESS_H
