#include <llama_debug/binary/binary.h>
#include <llama_debug/binary/parser.h>

#include <iostream>
#include <iomanip>

int main()
{
  using namespace llama_debug;

  // auto &bin = parse("C:\\Windows\\System32\\msvcrt.dll");
  auto &bin = parse("C:\\Windows\\System32\\cmd.exe");

  if (!bin) {
    std::cout << "Error loading binary!\n";
    return 1;
  }

  std::cout << *bin << "\n";

  return 0;
}