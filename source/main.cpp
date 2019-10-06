// 
// GCC Compile command: 
// g++ -I ./source ./source/main.cpp -lncursesw
// 

#include <iostream>
#include <string>

#include <program.h>
#include <registers.h>
#include <instructions.h>
#include <opParse.h>
#include <interface.h>

int main(int argc, char* argv[])
{
    initRegisters();
    loadprogram("testing/progBonus.hex");
    initSearchTree();

    while (programCounter > program.size())
    {
        void(* func)(opcode &code) = parseOpcode(program[programCounter].getBits(), &SearchTree);
        (* func)(program[programCounter]);
    }
    
    programCounter = 0;

    GUIrun();
}