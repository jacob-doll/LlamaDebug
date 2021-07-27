#include <llama_debug/binary/binary.h>
#include <llama_debug/binary/binary_pe.h>
#include <llama_debug/binary/binary_pe64.h>
#include <cstdio>

int main()
{
  using namespace llama_debug;
  binary *bin = binary::from_file("C:\\Windows\\System32\\cmd.exe");
  printf("%s\n", (bin->type() == BINARY_PE) ? "PE" : "PE64");
  bin->debug_print();
  printf("entry: %llx\n", bin->entry_point());
  printf("base: %llx\n", bin->base_addr());
  delete bin;
}