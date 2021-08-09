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

  // for (section sec : bin->sections()) {
  //   std::cout << sec.name << "\n";
  //   hex_out("Physical size: ", 8, sec.phys_size);
  //   hex_out("Virtual size: ", 8, sec.virt_size);
  //   hex_out("Virtual address: ", 16, sec.phys_addr);
  //   hex_out("Virtual address: ", 16, sec.virt_addr);
  //   hex_out("Flags: ", 8, sec.flags);
  // }

  for (symbol sym : bin->symbols()) {
    std::cout << sym.lib << ": " << sym.name << ": " << sym.address << "\n";
  }
  delete bin;
}