#ifndef LLAMADEBUG_BINARY_H
#define LLAMADEBUG_BINARY_H

#include <string>
#include <cstdint>

namespace LlamaDebug
{

class Binary
{
public:
    Binary() = default;
    virtual ~Binary();

    virtual bool FromFile(const std::string& Filename) = 0;
    virtual bool FromBuffer(const uint8_t* Buffer, uint32_t Size) = 0;

    virtual void DebugPrint() = 0;
private:
};

}; // LlamaDebug

#endif // LLAMADEBUG_BINARY_H
