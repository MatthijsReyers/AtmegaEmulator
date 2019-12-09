#pragma once

#include <string>
#include <sstream>
#include <iostream>

class settingsObj
{
    private:

    public:
        bool helpFlag = false;
        bool unitTestModeFlag = false;
        bool chipFlag = false;
        bool reverseAsmFlag = true;

        int chip = 0;
        std::string hexFile;

        void parse(int argc, char* argv[])
        {
            // Check if a file has been specified.
            if (argc == 1) throw "You need to specify a hex file to emulate.";

            // The file is always the last argument or a help flag.
            hexFile = std::string(argv[argc-1]);
            if (hexFile == "-h" || hexFile == "--help") helpFlag = true;

            // Parse all other arguments.
            std::string arg, next;
            for (int i = 1; i < argc - 1; i++)
            {
                //
                arg = std::string(argv[i]);
                next = std::string(argv[i+1]);

                // Deal with chip type
                if (arg == "-c" || arg == "--chip")
                {
                    chipFlag = true;

                    if (std::string(next) == "328P") chip = 0;
                    else if (next == "168PA") chip = 1;
                    else if (next == "88PA") chip = 2;
                    else if (next == "48PA") chip = 3;
                    else {
                        std::stringstream ss;
                        ss << "Unkown chip type: \"" << next << "\".";
                        throw ss.str().c_str();
                    }

                    // Skip next argument.
                    i++;
                }

                // Deal with reverse asm flag
                else if (arg == "-r" || arg == "--reverse-asm")
                {
                    if (next == "Y" || next == "y" || next == "YES" || next == "true") reverseAsmFlag = true;
                    else if (next == "N" || next == "n" || next == "NO" || next == "false") reverseAsmFlag = false;
                    else throw "Wrong option for reverse asm flag, please use \"Y\" or \"N\".";
                    i++;
                }

                // Deal with boolean only flags.
                else if (std::string(arg) == "-h" || arg == "--help") helpFlag = true;
                else if (arg == "-u") unitTestModeFlag = true;

                // That's a problem.
                else {
                    // std::stringstream ss;
                    // ss << "Unkown command line argument: \"" << arg << "\".";
                    // auto step1 = ss.str();
                    // auto step2 = step1.c_str();
                    // std::cout << step2 << "\n";
                    std::cout << arg << std::endl;
                    std::cout << (arg == "-h") << std::endl;

                    throw"Unkown command line argument:";
                }
            }
        }

        void showHelp()
        {
            printf("Usage: emu [OPTION]... [FILE]...\n\n");

            printf("Options:\n");
            printf("  -h, --help                 show this menu\n");
            printf("  -r, --reverse-asm {Y/N}    try to reverse asemble the provided hex file,\n");
            printf("                               (set to yes by default)\n");
            printf("  -c, --chip {MODEL}         set which Atmega chip to emulate, supported are: \n");
            printf("                               328P, 168PA, 88PA, 48PA\n");

            printf("\nDeveloper options:\n");
            printf("  -u                         run in unit test mode\n");
            printf("  -o {FILE}                  file to write debug output to\n");
        }
};

settingsObj settings;