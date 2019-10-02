#include <iostream>
#include <string>

#include <program.h>
#include <registers.h>
#include <instructions.h>
#include <opParse.h>

int main(int argc, char* argv[])
{
    initRegisters();
    std::string url = "progBonus.hex";
    loadprogram(url);

    addopcode(ADD, 0b000011, 6);
    addopcode(LDI, 0b1110, 4);
    addopcode(SBRC, 0b1111110, 7);

    void(* instruction)(opcode &code) = parseOpcode(program[0]);
    (* instruction)(program[0]);
    std::cout << std::bitset<16>(program[0].getBits()) << std::endl;

    return 0;

    for (int i = 0; i < 4; i++)
    {
        void(* instruction)(opcode &code) = parseOpcode(program[i]);
        (* instruction)(program[i]);
    }

    return 0;

    // LDI(program[0]);
    // LDI(program[1]);
    // LDI(program[2]);
    // SBRC(program[3]);
    // ADD(program[4]);
}