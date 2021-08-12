#include <llama_debug/binary/binary.h>
#include <llama_debug/binary/pe/binary_pe.h>
#include <iostream>
#include <iomanip>

static void hex_out(const std::string &out, std::streamsize prec, const uint64_t hex)
{
  std::cout << out
            << "0x"
            << std::hex
            << std::uppercase
            << std::setw(prec)
            << std::setfill('0')
            << hex
            << "\n";
}

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