#ifndef LLAMADEBUG_IMPORT_ENTRY_H
#define LLAMADEBUG_IMPORT_ENTRY_H

#include "llama_debug/binary/symbol.h"

namespace llama_debug {

struct raw_hint_name
{
  uint16_t hint;
  char name[1];
};

class pe_import_entry : public symbol
{
public:
  pe_import_entry();
  pe_import_entry(const std::string &lib,
    const raw_hint_name *data,
    uintptr_t address,
    uint64_t name_rva,
    uint64_t address_rva);
  pe_import_entry(const std::string &lib,
    uint16_t hint,
    const std::string &name,
    uintptr_t address,
    uint64_t name_rva,
    uint64_t address_rva);

  virtual std::string &lib() override;
  virtual std::string &name() override;
  virtual uintptr_t address() const override;

  uint16_t hint() const;
  uint64_t name_rva() const;
  uint64_t address_rva() const;

  void lib(const std::string &lib);
  void name(const std::string &name);
  void address(const uintptr_t address);
  void hint(const uint16_t hint);
  void name_rva(const uint64_t name_rva);
  void address_rva(const uint64_t address_rva);

  virtual std::ostream &print(std::ostream &os) const override;

private:
  std::string m_lib;
  std::string m_name;
  uintptr_t m_address;
  uint16_t m_hint;
  uint64_t m_name_rva;
  uint64_t m_address_rva;
};

}// namespace llama_debug


#endif// LLAMADEBUG_IMPORT_ENTRY_H
