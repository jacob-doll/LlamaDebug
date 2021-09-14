#include <iostream>
#include <exception>

#include <llama_debug/process/process.h>
#include <llama_debug/process/memory.h>
#include <llama_debug/binary/parser.h>

int main()
{
  using namespace llama_debug;
  std::cout << "Creating Process!\n";
  try {
    for (ldpid_t pid : process::enum_processes()) {
      std::cout << "PID: " << pid << "\n";
    }
  } catch (std::exception &e) {
    std::cout << e.what() << "\n";
  }

  return 0;
}