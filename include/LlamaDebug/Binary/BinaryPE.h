#ifndef LLAMADEBUG_BINARYPE_H
#define LLAMADEBUG_BINARYPE_H

#include <LlamaDebug/Binary/Binary.h>
#include <LlamaDebug/Binary/Defs/PE.h>

#include <string>

namespace LlamaDebug
{

class BinaryPE : public Binary
{
public:
    virtual ~BinaryPE();
    
    static bool Validate(const uint8_t* Buffer, uint32_t Size);

    virtual bool FromFile(const std::string& Filename) override;
    virtual bool FromBuffer(const uint8_t* Buffer, uint32_t Size) override;

    virtual void DebugPrint() override;

    virtual uintptr_t GetEntryPoint() override;

private:
    PEImageDosHeader m_DosHeader;
    PEImageNTHeaders32 m_Headers;
    PEImageSectionHeader *m_SectionHeaders;
};

} // namespace LlamaDebug

#endif // LLAMADEBUG_BINARYPE_H
