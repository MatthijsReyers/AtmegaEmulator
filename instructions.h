#pragma once

#include <iostream>

// #include <registers.h>
#include <optcode.h>
#include <program.h>
#include <registers.h>

// Add with carry.
void ADC(optcode &code) 
{
    // Parse optcode.
    short toAdd = 16 + ((code.getBits() & 0b0000000000001111) | ((code.getBits() & 0b0000001000000000) >> 4));
    short toAddTo = 16 + ((code.getBits() & 0b0000000111110000) >> 4);

    // Calculate result and add to register.
    int result = registers[toAddTo].getValue() + registers[toAdd].getValue();
    if (flagC) result++;
    registers[toAddTo].loadValue((short)result);

    // Half carry flag.
    flagH = (result >= 16);

    // S


    // V

    // N


    // Z zero flag.
    flagZ = (result == 0);

    // C carry flag.
    flagC = (result > 65536);
    
    // Increment program counter.
    programCounter++;

    // Print some stuff (temporary because UI is not done yet.)
    std::cout << "adc " << registers[toAddTo].name << ", " << registers[toAdd].name << std::endl;
}

// Add without carry.
void ADD(optcode &code) 
{
    // Parse optcode.
    short toAdd = 16 + ((code.getBits() & 0b0000000000001111) | ((code.getBits() & 0b0000001000000000) >> 4));
    short toAddTo = 16 + ((code.getBits() & 0b0000000111110000) >> 4);

    // Calculate result and add to register.
    int result = registers[toAddTo].getValue() + registers[toAdd].getValue();
    registers[toAddTo].loadValue((short)result);

    // Half carry flag.
    flagH = (result >= 16);

    // S


    // V

    // N


    // Z zero flag.
    flagZ = (result == 0);

    // C carry flag.
    flagC = (result > 65536);
    
    // Increment program counter.
    programCounter++;

    // Print some stuff (temporary because UI is not done yet.)
    std::cout << "add " << registers[toAddTo].name << ", " << registers[toAdd].name << std::endl;
}

// Logical AND.
void AND(optcode &code)
{
    // Parse optcode.
    short toAnd = 16 + ((code.getBits() & 0b0000000000001111) | ((code.getBits() & 0b0000001000000000) >> 5));
    short toAndTo = 16 + ((code.getBits() & 0b0000000111110000) >> 4);

    // Calcuate result and add to register.
    int result = registers[toAndTo].getValue() & registers[toAnd].getValue();
    registers[toAndTo].loadValue((short)result);

    // S

    // V
    flagV = false;

    // N

    // Z zeroflag
    flagZ = (result == 0);

    // Increment program counter.
    programCounter++;

    // Print some stuff (temporary because UI is not done yet.)
    std::cout << "and " << registers[toAndTo].name << ", " << registers[toAnd].name << std::endl;
}

// Logical AND with Immediate.
void ANDI(optcode &code)
{
    // Parse optcode.
    short toAnd = ((code.getBits() & 0b0000000000001111) | ((code.getBits() & 0b0000111100000000) >> 4));
    short toAndTo = 16 + ((code.getBits() & 0b0000000011110000) >> 4);

    // Calcuate result and add to register.
    int result = registers[toAndTo].getValue() & toAnd;
    registers[toAndTo].loadValue((short)result);

    // S

    // V (cleared)
    flagV = false;

    // N negativeFlag is bit 7.
    flagN = registers[toAndTo].getNthBit(7);

    // Z zeroflag
    flagZ = (result == 0);

    // Increment program counter.
    programCounter++;

    // Print some stuff (temporary because UI is not done yet.)
    std::cout << "and " << registers[toAndTo].name << ", " << registers[toAnd].name << std::endl;
}

// Arithmetic Shift Right.
void ASR(optcode &code)
{
    // Parse optcode.
    short toShift = 16 + ((code.getBits() & 0b0000000111110000) >> 4);

    // Carry flag is set to Bit 0.
    flagC = registers[toShift].getNthBit(0);

    // Bit 7 stays bit 7.
    short bit7 = registers[toShift].getNthBit(7);

    // Actually shift bits and add bit 7.
    short result = (registers[toShift].getValue() >> 1) | (bit7 << 7);
    registers[toShift].loadValue(result);

    // S

    // V

    // Negative flag is bit 7.
    flagN = registers[toShift].getNthBit(7);

    // Z = Zero flag
    flagZ = (result == 0);

    // Increment program counter.
    programCounter++;

    // Print some stuff (temporary because UI is not done yet.)
    std::cout << "asr " << registers[toShift].name << std::endl;
}

// Bit Clear in SREG.
void BCLR(optcode &code)
{
    // Parse optcode.
    short flagNum = ((code.getBits() & 0b0000000001110000) >> 4);

    switch (flagNum)
    {
        case 0: flagC = false; break;
        case 1: flagZ = false; break;
        case 2: flagN = false; break;
        case 3: flagV = false; break;
        case 4: flagS = false; break;
        case 5: flagH = false; break;
        case 6: flagT = false; break;
        case 7: flagI = false; break; 
        default: printf("MASSIVE ERROR!"); break;
    }

    // Increment program counter.
    programCounter++;

    // Print some stuff (temporary because UI is not done yet.)
    std::cout << "bclr " << flagNum << std::endl;
}

// Branch if Bit in SREG is Cleared
void BRBC(optcode &code)
{
    // Parse optcode.
    short flagNum = ((code.getBits() & 0b0000000000000111));
    short jumpAmount = ((code.getBits() & 0b0000001111111000) >> 3);
    
    // Convert jump amount.
    if (((jumpAmount & 0b01000000) >> 6) == 1) jumpAmount = (jumpAmount & 0b00111111) * -1;
    else jumpAmount = (jumpAmount & 0b00111111) - 1;

    bool flagState;
    switch (flagNum)
    {
        case 0: flagState = flagC; break;
        case 1: flagState = flagZ; break;
        case 2: flagState = flagN; break;
        case 3: flagState = flagV; break;
        case 4: flagState = flagS; break;
        case 5: flagState = flagH; break;
        case 6: flagState = flagT; break;
        case 7: flagState = flagI; break; 
        default: printf("MASSIVE ERROR!"); break;
    }

    if (flagState != true)
    {
        programCounter = programCounter + jumpAmount;
    }

    // Increment program counter.
    programCounter++;

    // Print some stuff (temporary because UI is not done yet.)
    std::cout << "brbc " << flagNum << ", " << jumpAmount << std::endl;
}

// Load immediate.
void LDI(optcode &code)
{
    // Extract value to be loaded into register.
    short toLoad = ((code.getBits() & 0b0000111100000000) >> 4) | (code.getBits() & 0b0000000000001111);

    // Identify register.
    int reg = ((code.getBits() & 0b0000000011110000) >> 4) + 16;

    // Load stuff into register.
    registers[reg].loadValue(toLoad);    

    // Increment program counter.
    programCounter++;

    // Print some stuff (temporary because UI is not done yet.)
    std::cout << "ldi " << registers[reg].name << ", " << toLoad << std::endl;
}
