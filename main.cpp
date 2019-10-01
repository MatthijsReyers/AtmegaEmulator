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
    
    LDI(program[0]);
    LDI(program[1]);
    LDI(program[2]);
    SBRC(program[3]);
    ADD(program[4]);
}