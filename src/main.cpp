#include <cstdio>

#include "LlamaDebug/Debugger.h"
#include "LlamaDebug/Binary/Binary.h"
#include "LlamaDebug/Binary/BinaryPE.h"
#include "LlamaDebug/Binary/BinaryPE64.h"
#include "LlamaDebug/Binary/Defs/PE.h"

void outputCallback(const char* output) 
{
    printf("%s", output);
}

using namespace LlamaDebug;

int main(int argc, char **argv) 
{

    if (argc < 2)
    {
        fprintf(stderr, "Not enough args!\n");
        return 1;
    }

    if (!Debugger::Instance().Open(argv[1])) 
    {
        printf("Couldn't create debugger!\n");
    }

    Debugger::Instance().Wait();
    
    uintptr_t ProcessBase = Debugger::Instance().GetProcessBase();
    uint32_t ProgramSize = Debugger::Instance().GetProgramSize();
    uint8_t* Buffer = new uint8_t[ProgramSize];
    Debugger::Instance().ReadMemory(ProcessBase, Buffer, ProgramSize);
    if (BinaryPE::Validate(Buffer, ProgramSize))
    {
        printf("Windows 32-bit\n");
        Binary *binary = new BinaryPE();
        binary->FromBuffer(Buffer, ProgramSize);
        Debugger::Instance().AddBreakpoint(binary->GetEntryPoint());
        delete binary;
        delete[] Buffer;
    } else if (BinaryPE64::Validate(Buffer, ProgramSize))
    {
        printf("Windows 64-bit\n");
        Binary *binary = new BinaryPE64();
        binary->FromBuffer(Buffer, ProgramSize);
        Debugger::Instance().AddBreakpoint(binary->GetEntryPoint());
        delete binary;
        delete[] Buffer;
    }

    int status;
    while ((status = Debugger::Instance().Wait()) != LD_STATUS_DEAD) 
    {
        if (status == LD_STATUS_CREATE_PROCESS) printf("Process base: %p\n", (void*)Debugger::Instance().GetProcessBase());
        if (status == LD_STATUS_LOAD_MODULE) continue;
        printf("> ");
        char buffer[512];
        fgets(buffer, 512, stdin);
    }

    // for (Module mod : Debugger::Instance().GetModules())
    // {
    //     printf("%s\n", mod.ModName.c_str());
    // }

    printf("Closing Debugger!\n");

    Debugger::Instance().Close();    

    return 0;
}