#include <llama_debug/Binary/BinaryPE64.h>
#include <fstream>
#include <cstdio>

namespace LlamaDebug
{

static uint32_t RVAToPhysical(PEImageSectionHeader *Sections, uint16_t NumSections, uint32_t RVA)
{
    for (uint16_t i = 0; i < NumSections; i++)
    {
        uint32_t SectionVirtualAddress = Sections[i].VirtualAddress;
        uint32_t SectionVirtualSize = Sections[i].Misc.VirtualSize;
        if (RVA >= SectionVirtualAddress && 
            RVA < SectionVirtualAddress + SectionVirtualSize)
        {
            return Sections[i].PointerToRawData + (RVA - SectionVirtualAddress);
        }
    }
    return RVA;
}

BinaryPE64::~BinaryPE64()
{
    delete[] m_SectionHeaders;
}

bool BinaryPE64::Validate(const uint8_t* Buffer, uint32_t Size)
{
    // Must have the default MS-Dos header
    if (Size < sizeof(PEImageDosHeader)) return false;
    // Check Dos Signature
    uint16_t DosSignature;
    std::memcpy(&DosSignature, Buffer, sizeof(DosSignature));
    if (DosSignature == IMAGE_DOS_SIGNATURE)
    {
        uint32_t ExeOffset;
        std::memcpy(&ExeOffset, Buffer + 0x3c, sizeof(ExeOffset));
        // Check NT Signature 0x00004550 (PE)
        uint32_t NTSignature;
        std::memcpy(&NTSignature, Buffer + ExeOffset, sizeof(NTSignature));
        if (NTSignature != IMAGE_NT_SIGNATURE) return false;
        // Check that executable is 64-Bit
        uint16_t OptionalMagic;
        std::memcpy(&OptionalMagic, Buffer + ExeOffset + 0x18, sizeof(OptionalMagic));
        if (OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) return true;
    }
    return false;
}

bool BinaryPE64::FromFile(const std::string& Filename) 
{ 
    std::ifstream BinaryFile(Filename, std::ios::in|std::ios::binary);
    if (!BinaryFile.is_open()) return false;

    BinaryFile.read((char*)(&m_DosHeader), sizeof(m_DosHeader));
    BinaryFile.seekg(m_DosHeader.e_lfanew, std::ios::beg);
    BinaryFile.read((char*)(&m_Headers), sizeof(m_Headers));

    m_SectionHeaders = new PEImageSectionHeader[m_Headers.FileHeader.NumberOfSections];

    for (uint16_t i = 0; i < m_Headers.FileHeader.NumberOfSections; i++)
    {
        BinaryFile.read((char*)(&m_SectionHeaders[i]), sizeof(PEImageSectionHeader));
    }

    

    BinaryFile.close();
    return true;
}

bool BinaryPE64::FromBuffer(const uint8_t* Buffer, uint32_t Size) { 
    if (!Validate(Buffer, Size)) return false;


    std::memcpy(&m_DosHeader, Buffer, sizeof(m_DosHeader));
    uint32_t Index = m_DosHeader.e_lfanew;
    std::memcpy(&m_Headers, Buffer + Index, sizeof(m_Headers));
    Index += sizeof(m_Headers);

    m_SectionHeaders = new PEImageSectionHeader[m_Headers.FileHeader.NumberOfSections];

    for (uint16_t i = 0; i < m_Headers.FileHeader.NumberOfSections; i++)
    {
        std::memcpy(&m_SectionHeaders[i], Buffer + Index, sizeof(PEImageSectionHeader));
        Index += sizeof(PEImageSectionHeader);
    } 

    return true; 
}

void BinaryPE64::DebugPrint()
{
    for (uint16_t i = 0; i < m_Headers.FileHeader.NumberOfSections; i++)
    {
        printf("%.8s\n", m_SectionHeaders[i].Name);
    } 
}

uintptr_t BinaryPE64::GetEntryPoint()
{
    return m_Headers.OptionalHeader.AddressOfEntryPoint + m_Headers.OptionalHeader.ImageBase;
}

} // namespace LlamaDebug
