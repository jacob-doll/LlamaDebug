#include <llama_debug/binary/binary.h>
#include <llama_debug/binary/binary_pe64.h>
#include <cstdio>

int main()
{
  using namespace llama_debug;
  binary_pe64 binary("C:\\Windows\\System32\\calc.exe");
  binary.debug_print();
  printf("entry: %p\n", (void *)binary.entry_point());
  printf("base: %p\n", (void *)binary.base_addr());
}