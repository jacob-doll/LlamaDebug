#include <cstdio>

#include "LlamaDebug/Debugger.h"

void outputCallback(const char* output) 
{
    printf("%s", output);
}

void inputCallback(const char* prompt, unsigned long bufferSize) 
{
    printf("getting input\n");
}

int main(int argc, char **argv) 
{

    if (argc < 2)
    {
        fprintf(stderr, "Not enough args!\n");
        return 1;
    }

    LlamaDebug::Debugger debugger;
    debugger.setOutputCallback(&outputCallback);

    if (!debugger.open(argv[1])) 
    {
        printf("Couldn't create debugger!\n");
    }

    int status;
    while ((status = debugger.wait()) != LD_STATUS_DEAD) 
    {
        if (status = LD_STATUS_LOAD_MODULE) continue;
        printf("> ");
        char buffer[512];
        fgets(buffer, 512, stdin);
    }

    debugger.close();    

    return 0;
}