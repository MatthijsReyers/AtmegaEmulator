#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <bitset> // May be removed later, just here for debug reasons.

#include <optcode.h>

// Global program vector that holds a list of all optcodes.
// -----------------------------------------------------------
std::vector<optcode> program;
int64_t programCounter;


// Why is this not a default function?
// -----------------------------------------------------------
int convertHex(char* hexStr)
{
    int out;
    std::stringstream ss;
    ss << hexStr[0] << hexStr[1];
    ss >> std::hex >> out;
    return out;
}


// Load progam function that parses the .hex file.
// -----------------------------------------------------------
void loadprogram(std::string &fileUrl)
{
    // Open file object.
    // -------------------------------------------------------
    std::ifstream programFile;
    programFile.open(fileUrl);

    // Skip first line (the program does not support initial 
    // segment offset, so will just ignore it for now).
    // -------------------------------------------------------
    programFile.seekg(18);
    
    // Parse first row.
    // -------------------------------------------------------
    char byteCount[2]; programFile.read(byteCount, 2);
    char address[4]; programFile.read(address, 4);
    char recordtype[2]; programFile.read(recordtype, 2);

    int byteCountReal = convertHex(byteCount) * 2;
    char optcodes[32]; programFile.read(optcodes, byteCountReal);
    for (int i = 0; i < (byteCountReal*2); i = i + 4)
    {
        int code = convertHex()
    }
    

    // while (programFile.good() && buffer[0] != ':')
    // {
    //     programFile.read(buffer, 1);

    //     std::cout << buffer << std::endl;
    // }

    // while (programFile.good())
    // {
    //     // Read two bytes (= chars) from file.
    //     programFile.read(optcodeBuffer, 2);

    //     // Convert bytes to one short.
    //     convertBuffer = (((short)optcodeBuffer[0]) << 8) | (0x00ff & optcodeBuffer[1]);

    //     std::cout << convertBuffer << std::endl;

    //     // Append to program vector.
    //     program.push_back(optcode(convertBuffer));
    // }
    // printf("done loading!\n");
}

void showProgram()
{
    for (auto &code : program)
    {
        short temp = code.getBits();
        std::cout << std::bitset<16>(temp) << std::endl;
    }
}

void loadDebugProgram()
{
    program.push_back(optcode(0b1110000000000000)); // LDI r16 0
    program.push_back(optcode(0b1110000000010011)); // LDI r17 3
    program.push_back(optcode(0b0001110000000001)); // ADC r16 r7
}
