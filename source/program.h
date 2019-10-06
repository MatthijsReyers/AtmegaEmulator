#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

#include <opcode.h>


// Global program vector that holds a list of all opcodes.
// And the global programCounter.
// -----------------------------------------------------------
std::vector<opcode> program;
int64_t programCounter = 0;


// Why is this not a default function?
// -----------------------------------------------------------
int convertHex(char* hexStr)
{
    int out;
    std::stringstream ss;
    ss << hexStr;
    ss >> std::hex >> out;
    return out;
}


// Load progam function that parses the .hex file.
// -----------------------------------------------------------
void loadprogram(const std::string &fileUrl)
{
    // Open file object.
    // -------------------------------------------------------
    std::ifstream programFile;
    programFile.open(fileUrl);

    // Read file line by line.
    // -------------------------------------------------------
    std::string line;
    while (std::getline(programFile, line))
    {
        // Check for start code, cause why not.
        if (line.at(0) != ':') throw "Missing a start code for line in hex file?";

        // Get byte count for row.
        char temp[5] = {line.at(1),line.at(2),0x00};
        int byteCount = convertHex(temp);

        // Determine the record type for the row.
        temp[0] = line.at(7);
        temp[1] = line.at(8);
        short recordType = convertHex(temp);

        // Take action based on the type of record.
        switch (recordType)
        {
            // Instructions will be added to program.
            case 0x00:
                for (int i = 0; i < byteCount*2; i = i+4)
                {
                    // Convert hex bytes to instruction bytes.
                    temp[0] = line.at(11+i);
                    temp[1] = line.at(12+i);
                    temp[2] = line.at(9+i);
                    temp[3] = line.at(10+i);
                    temp[4] = 0x00;
                    short instructionBytes = convertHex(temp);

                    // Add bytes to program.
                    program.push_back(opcode(instructionBytes));

                    // Support for 32 bit instructions.
                    if (instructionBytes & 0b1111111000000000 == 0b1001010000000000)
                    {
                        // Convert other hex bytes to instruction bytes.
                        temp[0] = line.at(15+i);
                        temp[1] = line.at(16+i);
                        temp[2] = line.at(13+i);
                        temp[3] = line.at(14+i);

                        // Make instruction 32 bit.
                        short instructionBytes = convertHex(temp);
                        int index = program.size() - 1;
                        program[index].make32bit(instructionBytes);

                        // Skip next two bytes.
                        i = i + 2;
                    }
                }
                break;
            
            // End of file. We're done here.
            case 0x01: return;

            // Other records types are not needed for this program, do nothing.
            default: break;
        }
    }
}

