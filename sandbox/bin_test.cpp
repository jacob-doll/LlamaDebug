#include <llama_debug/binary/binary.h>
#include <llama_debug/binary/parser.h>

#include <iostream>
#include <iomanip>

int main()
{
  using namespace llama_debug;

  auto &bin = parse("C:\\Windows\\System32\\msvcrt.dll");

  if (!bin) {
    std::cout << "Error loading binary!\n";
    return 1;
  }

  // std::cout << *bin << "\n";

  std::cout << "Printing symbols!\n";
  auto &symbols = bin->symbols();
  // for (auto &symbol : bin->symbols()) {
  //   std::cout << symbol->name() << "\n";
  // }

  return 0;
}