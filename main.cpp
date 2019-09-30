#include <iostream>
#include <string>

#include <program.h>
#include <registers.h>
#include <instructions.h>

int main(int argc, char* argv[])
{
    initRegisters();
    std::string url = "progBonus.hex";
    loadprogram(url);
    showProgram();

    loadDebugProgram();
    
    LDI(program[0]);
    LDI(program[1]);
    ADC(program[2]);
    
}