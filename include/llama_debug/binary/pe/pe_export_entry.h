#ifndef LLAMADEBUG_PE_EXPORT_ENTRY_H
#define LLAMADEBUG_PE_EXPORT_ENTRY_H

#include "llama_debug/binary/symbol.h"

namespace llama_debug {

union raw_export_address {
  uint32_t export_rva;
  uint32_t forwarder_rva;
};

class pe_export_entry : public symbol
{
public:
  pe_export_entry();
  pe_export_entry(const std::string &lib,
    const std::string &name,
    uintptr_t address,
    raw_export_address data,
    const std::string &forwarder_name,
    uint32_t name_rva,
    uint16_t ordinal);

  virtual std::string &lib() override;
  virtual std::string &name() override;
  virtual uintptr_t address() const override;

  uint32_t export_rva() const;
  uint32_t forwarder_rva() const;
  std::string &forwarder_name();
  uint32_t name_rva() const;
  uint16_t ordinal() const;

  void lib(const std::string &lib);
  void name(const std::string &name);
  void address(const uintptr_t address);
  void export_rva(const uint32_t export_rva);
  void forwarder_rva(const uint32_t forwarder_rva);
  void forwarder_name(const std::string &forwarder_name);
  void name_rva(const uint32_t name_rva);
  void ordinal(const uint16_t ordinal);

  virtual std::ostream &print(std::ostream &os) const override;

private:
  std::string m_lib;
  std::string m_name;
  uintptr_t m_address;

  uint32_t m_export_rva;
  uint32_t m_forwarder_rva;
  std::string m_forwarder_name;
  uint32_t m_name_rva;
  uint16_t m_ordinal;
};

}// namespace llama_debug

#endif// LLAMADEBUG_PE_EXPORT_ENTRY_H
