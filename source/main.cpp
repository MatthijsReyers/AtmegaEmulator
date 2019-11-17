// 
// GCC Compile command: 
// g++ -I ./source ./source/main.cpp -lncursesw
// 

#include <iostream>
#include <string>

#include <program.h>
#include <registers.h>
#include <flags.h>
#include <instructions.h>
#include <opParse.h>
#include <interface.h>

int main(int argc, char* argv[])
{
    initSearchTree();
    initRegisters();

    try 
    {
        loadprogram("tests/subRoutines/subRoutines.hex");
        
        // for (int i = 0; i < program.size(); i++)
        // {
        //     void(* func)(opcode &code) = parseOpcode(program[i], &SearchTree);
        //     (* func)(program[i]);
        // }
        
        programCounter = 0;
        resetRegisters();
        resetFlags();

        GUIrun();
    }
    
    catch (char const* msg) 
    {
        std::cout << "ERROR: " << msg << std::endl;
        exit(1);
    }
}
