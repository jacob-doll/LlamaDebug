#include <cstdio>

#include "LlamaDebug/Debugger.h"
#include "LlamaDebug/Binary/Binary.h"
#include "LlamaDebug/Binary/BinaryPE.h"

void outputCallback(const char* output) 
{
    printf("%s", output);
}

void inputCallback(const char* prompt, unsigned long bufferSize) 
{
    printf("getting input\n");
}

using namespace LlamaDebug;

int main(int argc, char **argv) 
{

    if (argc < 2)
    {
        fprintf(stderr, "Not enough args!\n");
        return 1;
    }

    // Binary* binary = new BinaryPE();
    // binary->FromFile(argv[1]);
    // binary->DebugPrint();
    // delete binary;

    if (!Debugger::Instance().Open(argv[1])) 
    {
        printf("Couldn't create debugger!\n");
    }

    int status;
    while ((status = Debugger::Instance().Wait()) != LD_STATUS_DEAD) 
    {
        printf("%x\n", Debugger::Instance().GetProcessBase());
        if (status = LD_STATUS_LOAD_MODULE) continue;
        if (status = LD_STATUS_CREATE_PROCESS)
        {
            printf("%x\n", Debugger::Instance().GetProcessBase());
        }
        // for(LlamaDebug::Module mod : debugger.GetModules())
        // {
        //     printf("%s\n", mod.file.c_str());
        // }
        printf("> ");
        char buffer[512];
        fgets(buffer, 512, stdin);
    }

    Debugger::Instance().Close();    

    return 0;
}