#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

// Status register bits
// ========================================
bool flagC; // Carry Flag
bool flagZ; // Zero Flag
bool flagN; // Negative Flag
bool flagV; // Two’s complement overflow indicator
bool flagS; // N ⊕ V, For signed tests
bool flagH; // Half Carry Flag
bool flagT; // Transfer bit used by BLD and BST instructions
bool flagI; // Global Interrupt Enable/Disable Flag


// Register struct/class
// ========================================
struct reg
{
    short mainbytes;
    std::string name;

    short getValue() {return this->mainbytes;}
    bool getNthBit(int n) {return (0b0001 & (mainbytes >> n));}
    std::string getName() {return this->name;}
    void setName(std::string nameToSet) {this->name = nameToSet;}
    void loadValue(short u) {this->mainbytes = u;}
};


// General purpose registers (r0-r31).
// ========================================
std::vector<reg> registers (32);
void initRegisters()
{
    std::stringstream nameStream;
    for (int i = 0; i < 32; i++)
    {
        // Empty string stream.
        nameStream.str(std::string());

        // Make register name.
        nameStream << "r" << i;
        std::string nameString = nameStream.str();

        // Populate register vector.
        registers[i] = reg();
        registers[i].setName(nameString);
    }
}