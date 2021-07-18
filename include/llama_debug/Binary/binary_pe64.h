#ifndef LLAMADEBUG_BINARYPE64_H
#define LLAMADEBUG_BINARYPE64_H

#include <llama_debug/binary/binary.h>
#include <llama_debug/binary/defs/PE.h>

namespace llama_debug
{

class binary_pe64 : public binary
{
public:
    virtual ~binary_pe64();
    
    static bool validate(const uint8_t* buffer, uint32_t size);

    virtual bool from_file(const std::string& filename) override;
    virtual bool from_buffer(const uint8_t* buffer, uint32_t size) override;

    virtual void debug_print() override;

    virtual uintptr_t entry_point() override;

private:
    PEImageDosHeader m_dos_headers;
    PEImageNTHeaders64 m_headers;
    PEImageSectionHeader *m_section_headers;
};

} // namespace llama_debug

#endif // LLAMADEBUG_BINARYPE64_H
