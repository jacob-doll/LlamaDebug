#ifndef LLAMADEBUG_DOS_HEADER_H
#define LLAMADEBUG_DOS_HEADER_H

#include <array>
#include <cstdint>

#include <llama_debug/binary/pe/defs.h>

namespace llama_debug {

class dos_header
{
public:
  dos_header();
  dos_header(const pe_image_dos_header *image_dos_header);

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
  std::array<uint16_t, 4> res() const;
  uint16_t oemid() const;
  uint16_t oeminfo() const;
  std::array<uint16_t, 10> res2() const;
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
  void res(const std::array<uint16_t, 4> &res);
  void oemid(const uint16_t oemid);
  void oeminfo(const uint16_t oeminfo);
  void res2(const std::array<uint16_t, 10> &res2);
  void lfanew(const uint32_t lfanew);

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
  std::array<uint16_t, 4> m_res;
  uint16_t m_oemid;
  uint16_t m_oeminfo;
  std::array<uint16_t, 10> m_res2;
  uint32_t m_lfanew;
};

}// namespace llama_debug

#endif// LLAMADEBUG_DOS_HEADER_H
