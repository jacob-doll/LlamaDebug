#include <iostream>
#include <exception>

#include <llama_debug/process/process.h>

int main()
{
  std::cout << "Creating Process!\n";
  try {
    auto proc = llama_debug::process::create_process("cmd.exe", "");
    std::cout << (char *)proc->std_out().read().data();
    std::cout << (char *)proc->std_out().read().data();
    std::cout << (char *)proc->std_out().read().data();

    proc->close();
  } catch (std::exception &e) {
    std::cout << e.what() << "\n";
  }

  return 0;
}