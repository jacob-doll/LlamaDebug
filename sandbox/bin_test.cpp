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
  binary *bin = binary::from_file("C:\\Windows\\System32\\cmd.exe");

  hex_out("Base address: ", 16, bin->base_addr());
  hex_out("Entry point: ", 16, bin->entry_point());

  std::cout << "Sections:\n";

  std::cout << bin->sections().size() << "\n";

  for (auto& sec : bin->sections()) {
    std::cout << sec->name() << "\n";
    hex_out("Physical size: ", 8, sec->physical_size());
    hex_out("Virtual size: ", 8, sec->virtual_size());
    hex_out("Physical address: ", 16, sec->physical_address());
    hex_out("Virtual address: ", 16, sec->virtual_address());
  }

  // for (symbol sym : bin->symbols()) {
  //   std::cout << sym.lib << ": " << sym.name << ": " << sym.address << "\n";
  // }
  delete bin;
}