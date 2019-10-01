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
    ss << hexStr;
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
    char byteCount[3]; programFile.read(byteCount, 2);
    char address[5]; programFile.read(address, 4);
    char recordtype[3]; programFile.read(recordtype, 2);

    // Read optcodes.
    // -------------------------------------------------------
    int byteCountReal = convertHex(byteCount) * 2;
    char optcodes[33]; programFile.read(optcodes, byteCountReal);
    for (int i = 0; i < (byteCountReal-1); i=i+4)
    {
        // Insert into program optcodes.
        char temp[5] = {optcodes[i+2],optcodes[i+3],optcodes[i],optcodes[i+1],0x00};
        program.push_back(optcode(convertHex(temp)));
    }
}


// Unused debug functions.
// -----------------------------------------------------------
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
