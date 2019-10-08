#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <bitset>

#include <opcode.h>
#include <program.h>
#include <registers.h>

#include <iostream>
#include <fstream>

// Add with carry.
void ADC(opcode &code) 
{
    // Parse opcode.
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

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "adc  " << registers[toAddTo].name << ", " << registers[toAdd].name;
    code.assembly = ss.str();
}

// Add without carry.
void ADD(opcode &code) 
{
    // Parse opcode.
    short toAdd = ((code.getBits() & 0b0000000000001111) | ((code.getBits() & 0b0000001000000000) >> 5));
    short toAddTo = ((code.getBits() & 0b0000000111110000) >> 4);

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

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "add  " << registers[toAddTo].name << ", " << registers[toAdd].name;
    code.assembly = ss.str();
}

// Logical AND.
void AND(opcode &code)
{
    // Parse opcode.
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

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "and  " << registers[toAndTo].name << ", " << registers[toAnd].name << std::endl;
    code.assembly = ss.str();
}

// Logical AND with Immediate.
void ANDI(opcode &code)
{
    // Parse opcode.
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

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "and  " << registers[toAndTo].name << ", " << registers[toAnd].name;
    code.assembly = ss.str();
}

// Arithmetic Shift Right.
void ASR(opcode &code)
{
    // Parse opcode.
    short toShift = (code.getBits() & 0b0000000111110000) >> 4;

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

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "asr  " << registers[toShift].name;
    code.assembly = ss.str();
}

// Bit Clear in SREG.
void BCLR(opcode &code)
{
    // Parse opcode.
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

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "bclr " << flagNum;
    code.assembly = ss.str();
}

// Branch if Bit in SREG is Cleared.
void BRBC(opcode &code)
{
    // Parse opcode.
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

    if (!flagState)
    {
        programCounter = programCounter + jumpAmount;
    }

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "brbc " << flagNum << ", " << jumpAmount;
    code.assembly = ss.str();
}

// Branch if not equal.
void BRNE(opcode &code)
{
    // Parse opcode.
    short toJump = (code.getBits() & 0b0000000111111000) >> 3;

    // Forward or backwards? (Take care of two's compliment).
    if (code.getNthBit(9)) toJump = (((~toJump) & 0b00111111) + 1) * -1;

    // Do the actual jumping.
    if (!flagZ) programCounter = programCounter + 1 + toJump;
    else programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "brne " << toJump;
    code.assembly = ss.str();
}

// Load immediate.
void LDI(opcode &code)
{
    // Extract value to be loaded into register.
    short toLoad = ((code.getBits() & 0b0000111100000000) >> 4) | (code.getBits() & 0b0000000000001111);

    // Identify register.
    short reg = ((code.getBits() & 0b0000000011110000) >> 4) + 16;

    // Load stuff into register.
    registers[reg].loadValue(toLoad);    

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "ldi  " << registers[reg].name << ", " << toLoad;
    code.assembly = ss.str();
}

// Logical Shift Right
void LSR(opcode &code)
{
    // Parse opcode.
    short toShift = ((code.getBits() & 0b0000000111110000) >> 4);

    // Carry is set if least significant bit of register is set before shift.
    flagC = registers[toShift].getNthBit(0);

    // Negative flag
    flagN = 0;

    // Shift bits in register.
    short shifted = registers[toShift].getValue() >> 1 & 0b0111111111111111;
    registers[toShift].loadValue(shifted);

    // V

    // S

    // Zero flag (determined after shift).
    flagZ = (registers[toShift].getValue() == 0);

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "lsr " << registers[toShift].name;
    code.assembly = ss.str();
}

// Relative jump
void RJMP(opcode &code)
{
    // Parse opcode.
    short toJump = (code.getBits() & 0b0000011111111111);

    // Forward or backwards? (Take care of two's compliment).
    if (code.getNthBit(11)) toJump = ((~toJump & 0b011111111111) + 1)*-1;
    
    // Ajust program counter to jump.
    programCounter = programCounter + 1 + toJump;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "rjmp " << toJump;
    code.assembly = ss.str();
}

// Skip if Bit in Register is Cleared.
void SBRC(opcode &code)
{
    // Parse opcode
    short bitNum = ((code.getBits() & 0b0000000000000111));
    short toCheck = ((code.getBits() & 0b0000000111110000) >> 4);

    // Get bit from register
    bool state = registers[toCheck].getNthBit(bitNum);

    // Skip instruction if register is clear;
    if (state = false) programCounter = programCounter + 2;
    else programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "sbrc " << registers[toCheck].name << ", " << bitNum;
    code.assembly = ss.str();
}
