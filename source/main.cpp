#include <iostream>
#include <string>

#include <program.h>
#include <registers.h>
#include <instructions.h>
#include <opParse.h>

int main(int argc, char* argv[])
{
    initRegisters();
    std::string url = "testing/progBonus.hex";
    loadprogram(url);

    for (int i = 0; i < program.size(); i++)
    {
        try 
        {
            void(* instruction)(opcode &code) = parseOpcode(program[i], &SearchTree);
            (* instruction)(program[i]);
            std::cout << program[i].assembly << std::endl;
        }
        catch (const char* err)
        {
            std::cout << "== invalid optcode ==\n";
        }
    }
}