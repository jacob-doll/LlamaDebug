#include <iostream>
#include <exception>

#include <llama_debug/process/process.h>

int main()
{
  std::cout << "Creating Process!\n";
  try {
    auto proc = llama_debug::process::create_process("powershell.exe", "");
    while (proc->is_active())
      ;

    std::cout << "Process Exited!\n";
    proc->close();
  } catch (std::exception &e) {
    std::cout << e.what() << "\n";
  }

  return 0;
}