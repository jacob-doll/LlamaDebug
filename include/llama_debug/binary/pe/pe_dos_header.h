#ifndef LLAMADEBUG_PE_DOS_HEADER_H
#define LLAMADEBUG_PE_DOS_HEADER_H

#include <array>
#include <cstdint>
#include <ostream>

namespace llama_debug {

struct raw_dos_header
{
  uint16_t magic;
  uint16_t cblp;
  uint16_t cp;
  uint16_t crlc;
  uint16_t cparhdr;
  uint16_t minalloc;
  uint16_t maxalloc;
  uint16_t ss;
  uint16_t sp;
  uint16_t csum;
  uint16_t ip;
  uint16_t cs;
  uint16_t lfarlc;
  uint16_t ovno;
  uint16_t res[4];
  uint16_t oemid;
  uint16_t oeminfo;
  uint16_t res2[10];
  uint32_t lfanew;
};

class pe_dos_header
{
public:
  using res_t = std::array<uint16_t, 4>;
  using res2_t = std::array<uint16_t, 10>;

  pe_dos_header();
  pe_dos_header(const raw_dos_header *data);

  uint16_t magic() const;
  uint16_t cblp() const;
  uint16_t cp() const;
  uint16_t crlc() const;
  uint16_t cparhdr() const;
  uint16_t minalloc() const;
  uint16_t maxalloc() const;
  uint16_t ss() const;
  uint16_t sp() const;
  uint16_t csum() const;
  uint16_t ip() const;
  uint16_t cs() const;
  uint16_t lfarlc() const;
  uint16_t ovno() const;
  res_t &res();
  uint16_t oemid() const;
  uint16_t oeminfo() const;
  res2_t &res2();
  uint32_t lfanew() const;

  void magic(const uint16_t magic);
  void cblp(const uint16_t cblp);
  void cp(const uint16_t cp);
  void crlc(const uint16_t crlc);
  void cparhdr(const uint16_t cparhdr);
  void minalloc(const uint16_t minalloc);
  void maxalloc(const uint16_t maxalloc);
  void ss(const uint16_t ss);
  void sp(const uint16_t sp);
  void csum(const uint16_t csum);
  void ip(const uint16_t ip);
  void cs(const uint16_t cs);
  void lfarlc(const uint16_t lfarlc);
  void ovno(const uint16_t ovno);
  void res(const res_t &res);
  void oemid(const uint16_t oemid);
  void oeminfo(const uint16_t oeminfo);
  void res2(const res2_t &res2);
  void lfanew(const uint32_t lfanew);

  friend std::ostream &operator<<(std::ostream &os, const pe_dos_header &header);

private:
  uint16_t m_magic;
  uint16_t m_cblp;
  uint16_t m_cp;
  uint16_t m_crlc;
  uint16_t m_cparhdr;
  uint16_t m_minalloc;
  uint16_t m_maxalloc;
  uint16_t m_ss;
  uint16_t m_sp;
  uint16_t m_csum;
  uint16_t m_ip;
  uint16_t m_cs;
  uint16_t m_lfarlc;
  uint16_t m_ovno;
  res_t m_res;
  uint16_t m_oemid;
  uint16_t m_oeminfo;
  res2_t m_res2;
  uint32_t m_lfanew;
};

}// namespace llama_debug

#endif// LLAMADEBUG_PE_DOS_HEADER_H
