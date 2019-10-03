#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <bitset> // May be removed later, just here for debug reasons.

#include <opcode.h>

// Global program vector that holds a list of all opcodes.
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

    // Read opcodes.
    // -------------------------------------------------------
    int byteCountReal = convertHex(byteCount) * 2;
    char opcodes[33]; programFile.read(opcodes, byteCountReal);
    for (int i = 0; i < (byteCountReal-1); i=i+4)
    {
        // Insert into program opcodes.
        char temp[5] = {opcodes[i+2],opcodes[i+3],opcodes[i],opcodes[i+1],0x00};
        int bytes = convertHex(temp);
        program.push_back(opcode(bytes));

        // Support for 32 bit instructions.
        if (bytes & 0b1111111000000000 == 0b1001010000000000)
        {
            i = i + 4;
            char temp[5] = {opcodes[i+2],opcodes[i+3],opcodes[i],opcodes[i+1],0x00};
            int bytes = convertHex(temp);
            int index = program.size() - 1;
            program[index].make32bit(bytes);
        }
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
    program.push_back(opcode(0b1110000000000000)); // LDI r16 0
    program.push_back(opcode(0b1110000000010011)); // LDI r17 3
    program.push_back(opcode(0b0001110000000001)); // ADC r16 r7
}
