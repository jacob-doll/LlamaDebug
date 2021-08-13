#include <llama_debug/binary/binary.h>
#include <llama_debug/binary/pe/pe_binary.h>
#include <iostream>
#include <iomanip>

int main()
{
  using namespace llama_debug;
  binary *bin = binary::from_file("C:\\Windows\\System32\\msvcrt.dll");

  std::cout << *bin << "\n";

  // for (symbol sym : bin->symbols()) {
  //   std::cout << sym.lib << ": " << sym.name << ": " << sym.address << "\n";
  // }
  delete bin;
}