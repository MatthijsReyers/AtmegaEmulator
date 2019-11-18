#pragma once

#include <settings.h>

#include <string>
#include <sstream>
#include <vector>
#include <iostream>


// Register struct/class.
// ===========================================================
class reg
{
    private:
        short mainBits = 0;
        std::string name;

    public:
        // Constructor.
        reg(std::string regName) {this->name = regName;}

        // Getters.
        auto getValue() {return this->mainBits;}
        auto getNthBit(short n) {return (bool)(0b01 & (mainBits >> n));}
        auto getName() {return this->name;}

        // Setters.
        void setValue(short u) {this->mainBits = u & 0b011111111;}
        void setNthBit(short n, short b) {this->mainBits = (mainBits & ((0b11111111011111111 >> (8-n))) | ((0b01 & b) << n) & 0b11111111);}
        void setName(std::string newName){this->name = newName;}
};


// Functions for initializing and resetting the registers.
// ===========================================================
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

    // Reserved registers. (0x58 - 0x5C).
    // --------------------------------------------------------
    for (int i = 0; i < 5; i++) registers.push_back(reg(reserved));

    registers.push_back(reg("SPL"));    // 0x5D
    registers.push_back(reg("SPH"));    // 0x5E
    registers.push_back(reg("SREG"));   // 0x5F

    // 160 Ext I/O Reg. (Not used on most adruino's).
    // --------------------------------------------------------
    for (int i = 0; i < 160; i++)
    {
        registers.push_back(reg(reserved));
    }

    // Internal SRAM. (Size depends on chip's model).
    // --------------------------------------------------------
    switch (settings.chip)
    {
        // Atmega 328P (2048 bytes of internal SRAM)
        default:
        case 0:
            for (int i = 0x0100; i <= 0x08FF; i++) registers.push_back(reg(reserved));
            break;
        
        // Atmega 168PA and 88PA (1024 bytes of internal SRAM)
        case 1:
        case 2:
            for (int i = 0x0100; i <= 0x04FF; i++) registers.push_back(reg(reserved));
            break;

        // Atmega 48PA (512 bytes of internal SRAM)
        case 3:
            for (int i = 0x0100; i <= 0x02FF; i++) registers.push_back(reg(reserved));
            break;
    }
}

void resetRegisters()
{
    for (auto &item : registers)
    {
        item.setValue(0);
    }
}

