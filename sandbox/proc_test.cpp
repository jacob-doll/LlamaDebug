#include <iostream>
#include <exception>

#include <llama_debug/process/process.h>
#include <llama_debug/process/memory.h>

int main()
{
  using namespace llama_debug;
  std::cout << "Creating Process!\n";
  try {
    auto proc = process::create_process("calc.exe", "");
    for (mapped_region reg : proc->mapped_regions()) {
      std::cout << std::hex
                << reg.base_addr << "-" << reg.base_addr + reg.region_size << ":"
                << reg.mapped_file_name << "\n";
    }

    proc->close();
  } catch (std::exception &e) {
    std::cout << e.what() << "\n";
  }

  return 0;
}