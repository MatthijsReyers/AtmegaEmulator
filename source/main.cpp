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

    for (int i = 0; i < program.size(); i++)
    {
        void(* func)(opcode &code) = parseOpcode(program[i], &SearchTree);
        (* func)(program[i]);
        std::cout << program[i].assembly << "\n";
    }
    
    programCounter = 0;

    GUIrun();
}