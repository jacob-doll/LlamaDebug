#include <cstdio>

#include "LlamaDebug/Debugger.h"

int main(int argc, char **argv) {

    LlamaDebug::Debugger debugger;

    if (!debugger.open("calc.exe")) {
        printf("Couldn't create debugger!\n");
    }

    while (debugger.wait() != LD_STATUS_DEAD) {
        // printf("waitforevent\n");
    }

    debugger.close();    

    return 0;
}