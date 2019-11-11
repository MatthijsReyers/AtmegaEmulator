#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>
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
class reg
{
    private:
        int mainBits = 0;
        std::string name;


    public:
        // Constructor.
        reg(std::string regName) {this->name = regName;}

        // Getters.
        auto getValue() {return this->mainBits;}
        auto getNthBit(int n) {return (0b01 & (mainBits >> n));}
        auto getName() {return this->name;}

        // Setters.
        void setValue(short u) {this->mainBits = u;}
};


//
// ========================================
std::vector<reg> registers;
void initRegisters()
{
    std::string reserved = "reserved";

    // Create the general purpose registers (r0-r31).
    // --------------------------------------------------------
    std::stringstream nameStream;
    for (int i = 0; i < 32; i++)
    {
        // Empty string stream & make register name.
        nameStream.str("");
        nameStream << "r" << i;
        std::string nameString = nameStream.str();

        // Populate register vector.
        registers.push_back(nameString);
    }

    // Reserved registers. (0x20 - 0x22).
    // --------------------------------------------------------
    for (int i = 0; i < 3; i++) registers.push_back(reg(reserved));

    // General purpose I/O registers.
    // --------------------------------------------------------
    registers.push_back(reg("PINB"));   // 0x23
    registers.push_back(reg("DDRB"));   // 0x24
    registers.push_back(reg("PORTB"));  // 0x25
    registers.push_back(reg("PINC"));   // 0x26
    registers.push_back(reg("DDRC"));   // 0x27
    registers.push_back(reg("PORTC"));  // 0x28
    registers.push_back(reg("PIND"));   // 0x29
    registers.push_back(reg("DDRD"));   // 0x2A
    registers.push_back(reg("PORTD"));  // 0x2B

    // Reserved registers. (0x2C - 0x34).
    // --------------------------------------------------------
    for (int i = 0; i < 9; i++) registers.push_back(reg(reserved));

    // TIFR registers (no clue what they're for).
    // --------------------------------------------------------
    registers.push_back(reg("TIFR0"));  // 0x35
    registers.push_back(reg("TIFR1"));  // 0x36
    registers.push_back(reg("TIFR2"));  // 0x37

    // Reserved registers. (0x38 - 0x3A).
    // --------------------------------------------------------
    for (int i = 0; i < 3; i++) registers.push_back(reg(reserved));

    // More IO registers.
    // --------------------------------------------------------
    registers.push_back(reg("PCIFR"));  // 0x3B
    registers.push_back(reg("EIFR"));   // 0x3C
    registers.push_back(reg("EIMSK"));  // 0x3D
    registers.push_back(reg("GPIOR0")); // 0x3E
    registers.push_back(reg("EECR"));   // 0x3F
    registers.push_back(reg("EEDR"));   // 0x40
    registers.push_back(reg("EEARL"));  // 0x41
    registers.push_back(reg("EEARH"));  // 0x42
    registers.push_back(reg("GTCCR"));
    registers.push_back(reg("TCCR0A"));
    registers.push_back(reg("TCCR0B"));
    registers.push_back(reg("TCNT0"));
    registers.push_back(reg("OCR0A"));
    registers.push_back(reg("OCR0B"));

    registers.push_back(reg(reserved)); // 0x49

    registers.push_back(reg("GPIOR1"));
    registers.push_back(reg("GPIOR2"));

    registers.push_back(reg("SPCR"));
    registers.push_back(reg("SPSR"));
    registers.push_back(reg("SPDR"));

    registers.push_back(reg(reserved)); // 0x4F

    registers.push_back(reg("ACSR"));

    registers.push_back(reg(reserved)); // 0x51
    registers.push_back(reg(reserved)); // 0x52

    registers.push_back(reg("SMCR"));
    registers.push_back(reg("MCUSR"));
    registers.push_back(reg("MCUCR"));

    registers.push_back(reg(reserved)); // 0x56

    registers.push_back(reg("SPMCSR")); // 0x57

    // Reserved registers. (0x38 - 0x3A).
    // --------------------------------------------------------
    for (int i = 0; i < 5; i++) registers.push_back(reg(reserved));

}

void resetRegisters()
{
    for (auto &item : registers)
    {
        item.setValue(0);
    }
}