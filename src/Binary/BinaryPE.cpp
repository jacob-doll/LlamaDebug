#include <LlamaDebug/Binary/BinaryPE.h>
#include <fstream>
#include <cstdio>

namespace LlamaDebug
{

BinaryPE::~BinaryPE()
{
    delete[] m_SectionHeaders;
}

bool BinaryPE::FromFile(const std::string& Filename) 
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

bool BinaryPE::FromBuffer(const uint8_t* Buffer, uint32_t Size) { return true; }

void BinaryPE::DebugPrint()
{
    for (uint16_t i = 0; i < m_Headers.FileHeader.NumberOfSections; i++)
    {
        printf("%.8s\n", m_SectionHeaders[i].Name);
    } 
}

} // namespace LlamaDebug
