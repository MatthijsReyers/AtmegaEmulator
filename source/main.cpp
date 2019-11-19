// 
// GCC Compile command: 
// g++ -I ./source ./source/main.cpp -lncursesw
// 

#include <iostream>
#include <string>

#include <settings.h>
#include <program.h>
#include <registers.h>
#include <flags.h>
#include <instructions.h>
#include <opParse.h>
#include <interface.h>

int main(int argc, char* argv[])
{
    try 
    {
        // Parse command line arguments.
        settings.parse(argc, argv);

        // Help flags overrules all others.
        if (settings.helpFlag)
        {
            settings.showHelp();
            return 0;
        }

        // Load program from hex file.
        loadprogram(settings.hexFile);
        if (program.size() == 0) throw "Could not load a valid program from specified file.";
        
        // Initialize stuff.
        initRegisters();
        initStack();
        initSearchTree();

        // Unit test mode
        if (settings.unitTestModeFlag)
        {
            throw "Sorry unit test mode is currently not supported.";
        }

        // Try to reverse assembly.
        if (settings.reverseAsmFlag)
        {
            for (int i = 0; i < program.size(); i++)
            {
                void(* func)(opcode &code) = parseOpcode(program[i], &SearchTree);
                (* func)(program[i]);
            }
            programCounter = 0;
            resetRegisters();
            resetFlags();
            resetStack();
        }
        
        // Begin the UI/userinput loop.
        GUIrun();
    }
    
    catch (char const* msg) 
    {
        std::cout << "ERROR: " << msg << std::endl;
        return 1;
    }
}
