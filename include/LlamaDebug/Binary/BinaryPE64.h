#ifndef LLAMADEBUG_BINARYPE64_H
#define LLAMADEBUG_BINARYPE64_H

#include <LlamaDebug/Binary/Binary.h>
#include <LlamaDebug/Binary/Defs/PE.h>

namespace LlamaDebug
{

class BinaryPE64 : public Binary
{
public:
    virtual ~BinaryPE64();
    
    static bool Validate(const uint8_t* Buffer, uint32_t Size);

    virtual bool FromFile(const std::string& Filename) override;
    virtual bool FromBuffer(const uint8_t* Buffer, uint32_t Size) override;

    virtual void DebugPrint() override;

    virtual uintptr_t GetEntryPoint() override;

private:
    PEImageDosHeader m_DosHeader;
    PEImageNTHeaders64 m_Headers;
    PEImageSectionHeader *m_SectionHeaders;
};

} // namespace LlamaDebug

#endif // LLAMADEBUG_BINARYPE64_H
