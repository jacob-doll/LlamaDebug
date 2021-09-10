#include <iostream>
#include <exception>

#include <llama_debug/process/process.h>
#include <llama_debug/process/memory.h>
#include <llama_debug/binary/pe/pe_dos_header.h>

int main()
{
  using namespace llama_debug;
  std::cout << "Creating Process!\n";
  try {
    auto proc = process::create_process("cmd.exe", "");

    uintptr_t base_addr = proc->base_addr();
    raw_dos_header dos_header_{};
    proc->read_memory(base_addr, dos_header_);
    pe_dos_header dos_header{ &dos_header_ };
    std::cout << dos_header << "\n";

    proc->close();
  } catch (std::exception &e) {
    std::cout << e.what() << "\n";
  }

  return 0;
}