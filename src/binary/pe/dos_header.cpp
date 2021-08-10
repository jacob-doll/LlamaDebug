#include <llama_debug/binary/pe/dos_header.h>

#include <algorithm>

namespace llama_debug {

dos_header::dos_header()
  : m_magic{ 0x5A4D },
    m_cblp{ 0x0090 },
    m_cp{ 0x0003 },
    m_crlc{ 0x0000 },
    m_cparhdr{ 0x0004 },
    m_minalloc{ 0x0000 },
    m_maxalloc{ 0xFFFF },
    m_ss{ 0x0000 },
    m_sp{ 0x00B8 },
    m_csum{ 0x0000 },
    m_ip{ 0x0000 },
    m_cs{ 0x0000 },
    m_lfarlc{ 0x0040 },
    m_ovno{ 0x0000 },
    m_res{},
    m_oemid{ 0x0000 },
    m_oeminfo{ 0x0000 },
    m_res2{},
    m_lfanew{ 0x000000F0 }

{}

dos_header::dos_header(
  const raw_dos_header *data)
  : m_magic{ data->magic },
    m_cblp{ data->cblp },
    m_cp{ data->cp },
    m_crlc{ data->crlc },
    m_cparhdr{ data->cparhdr },
    m_minalloc{ data->minalloc },
    m_maxalloc{ data->maxalloc },
    m_ss{ data->ss },
    m_sp{ data->sp },
    m_csum{ data->csum },
    m_ip{ data->ip },
    m_cs{ data->cs },
    m_lfarlc{ data->lfarlc },
    m_ovno{ data->ovno },
    m_res{},
    m_oemid{ data->oemid },
    m_oeminfo{ data->oeminfo },
    m_res2{},
    m_lfanew{ data->lfanew }

{
  std::copy(
    std::begin(data->res),
    std::end(data->res),
    m_res.begin());
  std::copy(
    std::begin(data->res2),
    std::end(data->res2),
    m_res2.begin());
}

uint16_t dos_header::magic() const
{
  return m_magic;
}

uint16_t dos_header::cblp() const
{
  return m_cblp;
}

uint16_t dos_header::cp() const
{
  return m_cp;
}

uint16_t dos_header::crlc() const
{
  return m_crlc;
}

uint16_t dos_header::cparhdr() const
{
  return m_cparhdr;
}

uint16_t dos_header::minalloc() const
{
  return m_minalloc;
}

uint16_t dos_header::maxalloc() const
{
  return m_maxalloc;
}

uint16_t dos_header::ss() const
{
  return m_ss;
}

uint16_t dos_header::sp() const
{
  return m_sp;
}

uint16_t dos_header::csum() const
{
  return m_csum;
}

uint16_t dos_header::ip() const
{
  return m_ip;
}

uint16_t dos_header::cs() const
{
  return m_cs;
}

uint16_t dos_header::lfarlc() const
{
  return m_lfarlc;
}

uint16_t dos_header::ovno() const
{
  return m_ovno;
}

std::array<uint16_t, 4> dos_header::res() const
{
  return m_res;
}

uint16_t dos_header::oemid() const
{
  return m_oemid;
}

uint16_t dos_header::oeminfo() const
{
  return m_oeminfo;
}

std::array<uint16_t, 10> dos_header::res2() const
{
  return m_res2;
}

uint32_t dos_header::lfanew() const
{
  return m_lfanew;
}

void dos_header::magic(const uint16_t magic)
{
  m_magic = magic;
}

void dos_header::cblp(const uint16_t cblp)
{
  m_cblp = cblp;
}

void dos_header::cp(const uint16_t cp)
{
  m_cp = cp;
}

void dos_header::crlc(const uint16_t crlc)
{
  m_crlc = crlc;
}

void dos_header::cparhdr(const uint16_t cparhdr)
{
  m_cparhdr = cparhdr;
}

void dos_header::minalloc(const uint16_t minalloc)
{
  m_minalloc = minalloc;
}

void dos_header::maxalloc(const uint16_t maxalloc)
{
  m_maxalloc = maxalloc;
}

void dos_header::ss(const uint16_t ss)
{
  m_ss = ss;
}

void dos_header::sp(const uint16_t sp)
{
  m_sp = sp;
}

void dos_header::csum(const uint16_t csum)
{
  m_csum = csum;
}

void dos_header::ip(const uint16_t ip)
{
  m_ip = ip;
}

void dos_header::cs(const uint16_t cs)
{
  m_cs = cs;
}

void dos_header::lfarlc(const uint16_t lfarlc)
{
  m_lfarlc = lfarlc;
}

void dos_header::ovno(const uint16_t ovno)
{
  m_ovno = ovno;
}

void dos_header::res(const res_t &res)
{
  m_res = res;
}

void dos_header::oemid(const uint16_t oemid)
{
  m_oemid = oemid;
}

void dos_header::oeminfo(const uint16_t oeminfo)
{
  m_oeminfo = oeminfo;
}

void dos_header::res2(const res2_t &res2)
{
  m_res2 = res2;
}

void dos_header::lfanew(const uint32_t lfanew)
{
  m_lfanew = lfanew;
}


}// namespace llama_debug
