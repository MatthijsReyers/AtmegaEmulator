#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <bitset>

#include <opcode.h>
#include <program.h>
#include <registers.h>
#include <flags.h>


void BREAK(opcode &code)
{
    exit(0);
}

// Add with carry.
void ADC(opcode &code) 
{
    // Parse opcode.
    short toAdd = ((code.getBits() & 0b0000000000001111) | ((code.getBits() & 0b0000001000000000) >> 4));
    short toAddTo = ((code.getBits() & 0b0000000111110000) >> 4);

    // Calculate result and add to register.
    int previousValue = registers[toAddTo].getValue();
    int result = previousValue + registers[toAdd].getValue();
    if (flags.getC()) result++;
    registers[toAddTo].setValue(result);

    // H - Half carry flag.
    // flags.setH(result >= 16);

    // V
    // flags.setV(1);

    // N - Negative flag.
    flags.setN(registers[toAddTo].getNthBit(7));

    // Z - Zero flag.
    flags.setZ(result == 0);

    // C - carry flag.
    flags.setC(result > 255);

    // S - Sign flag.
    flags.setN(flags.getV() != flags.getN());
    
    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "adc  " << registers[toAddTo].getName() << ", " << registers[toAdd].getName();
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
    registers[toAddTo].setValue((short)result);

    // Half carry flag.
    // flags.setH(result >= 16);

    // V

    // N - Negative flag.
    flags.setN(registers[toAddTo].getNthBit(7));

    // Z - zero flag.
    flags.setZ(result == 0);

    // C - carry flag.
    flags.setC(result > 255);

    // S - Sign flag.
    flags.setN(flags.getV() != flags.getN());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "add  " << registers[toAddTo].getName() << ", " << registers[toAdd].getName();
    code.assembly = ss.str();
}

// Add immediate to Word.
void ADIW(opcode &code)
{
    programCounter++;
}

// Logical AND.
void AND(opcode &code)
{
    // Parse opcode.
    short toAnd = 16 + ((code.getBits() & 0b0000000000001111) | ((code.getBits() & 0b0000001000000000) >> 5));
    short toAndTo = 16 + ((code.getBits() & 0b0000000111110000) >> 4);

    // Calcuate result and add to register.
    int result = registers[toAndTo].getValue() & registers[toAnd].getValue();
    registers[toAndTo].setValue(result);

    // V (cleared)
    flags.setV(0);

    // N - Negative flag.
    flags.setN(registers[toAndTo].getNthBit(7));

    // Z - Zeroflag
    flags.setZ(result == 0);

    // S - Sign flag.
    flags.setN(flags.getV() != flags.getN());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "and  " << registers[toAndTo].getName() << ", " << registers[toAnd].getName() << std::endl;
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
    registers[toAndTo].setValue(result);

    // V - Overflow flag (cleared).
    flags.setV(0);

    // N - Negative flag is bit 7.
    flags.setN(registers[toAndTo].getNthBit(7));

    // Z - Zero flag.
    flags.setZ(result == 0);

    // S - Sign flag.
    flags.setN(flags.getV() != flags.getN());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "and  " << registers[toAndTo].getName() << ", " << registers[toAnd].getName();
    code.assembly = ss.str();
}

// Arithmetic Shift Right.
void ASR(opcode &code)
{
    // Parse opcode.
    short toShift = (code.getBits() & 0b0000000111110000) >> 4;

    // Carry flag is set to Bit 0.
    flags.setC(registers[toShift].getNthBit(0));

    // Bit 7 stays bit 7.
    short bit7 = registers[toShift].getNthBit(7);

    // Actually shift bits and add bit 7.
    short result = (registers[toShift].getValue() >> 1) | (bit7 << 7);
    registers[toShift].setValue(result);

    // V - Overflow flag.
    // flags.setV();

    // N - Negative flag is bit 7.
    flags.setN(registers[toShift].getNthBit(7));

    // Z - Zero flag.
    flags.setZ(result == 0);

    // S - Sign flag.
    flags.setN(flags.getV() != flags.getN());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "asr  " << registers[toShift].getName();
    code.assembly = ss.str();
}

// (Set) Bit Clear in SREG.
void BCLR(opcode &code)
{
    // Parse opcode.
    short flagNum = ((code.getBits() & 0b0000000001110000) >> 4);

    // Change the bit in SREG
    registers[0x5F].setNthBit(flagNum,false);

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "bclr " << flagNum;
    code.assembly = ss.str();
}

// Bit load from the T flag in SREG to a bit in register.
void BLD(opcode &code)
{
    // Parse opcode.
    short regToLoad = (code.getBits() & 0b00111110000) >> 4;
    short bitToLoad = (code.getBits() & 0b0111);

    //
    registers[regToLoad].setNthBit(bitToLoad, flags.getT());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "bld r" << regToLoad << ", " << bitToLoad;
    code.assembly = ss.str();
}

// Branch if Bit in SREG is Cleared.
void BRBC(opcode &code)
{
    // Parse opcode.
    short flagNum = ((code.getBits() & 0b0000000000000111));
    short jumpAmount = ((code.getBits() & 0b0000001111111000) >> 3);
    
    // Forward or backwards? (Take care of two's compliment).
    if (code.getNthBit(9)) jumpAmount = (((~jumpAmount) & 0b00111111) + 1) * -1;

    // Jump if needed.
    bool flagState =  (bool)registers[0x5F].getNthBit(flagNum);
    if (!flagState) programCounter = programCounter + jumpAmount;

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    switch (flagNum)
    {
        case 0: ss << "brsh " << jumpAmount; break;
        case 1: ss << "brne " << jumpAmount; break;
        case 2: ss << "brpl " << jumpAmount; break;
        case 3: ss << "brvc " << jumpAmount; break;
        case 4: ss << "brge " << jumpAmount; break;
        case 5: ss << "brhc " << jumpAmount; break;
        case 6: ss << "brtc " << jumpAmount; break;
        case 7: ss << "brid " << jumpAmount; break;

        default: ss << "brbc " << flagNum << ", " << jumpAmount; break;
    }
    code.assembly = ss.str();
}

// Branch if Bit in SREG is Set.
void BRBS(opcode &code)
{
    // Parse opcode.
    short flagNum = ((code.getBits() & 0b0000000000000111));
    short jumpAmount = ((code.getBits() & 0b0000001111111000) >> 3);
    
    // Forward or backwards? (Take care of two's compliment).
    if (code.getNthBit(9)) jumpAmount = (((~jumpAmount) & 0b00111111) + 1) * -1;

    // Jump if needed.
    if (registers[0x5F].getNthBit(flagNum)) programCounter = programCounter + jumpAmount;

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    switch (flagNum)
    {
        case 0: ss << "brlo " << jumpAmount; break;
        case 1: ss << "breq " << jumpAmount; break;
        case 2: ss << "brmi " << jumpAmount; break;
        case 3: ss << "brvs " << jumpAmount; break;
        case 4: ss << "brlt " << jumpAmount; break;
        case 5: ss << "brhs " << jumpAmount; break;
        case 6: ss << "brts " << jumpAmount; break;
        case 7: ss << "brie " << jumpAmount; break;
    
        default: ss << "brbs " << flagNum << ", " << jumpAmount; break;
    }
    code.assembly = ss.str();
}

// Bit set in SREG.
void BSET(opcode &code)
{
    // Parse opcode.
    short flagNum = ((code.getBits() & 0b0000000001110000) >> 4);

    // Change the bit in SREG
    registers[0x5F].setNthBit(flagNum,1);

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "bset " << flagNum;
    code.assembly = ss.str();
}

// Bit store from bit in Register to T flag.
void BST(opcode &code)
{
    programCounter++;
}

// Long Call to a Subroutine.
void CALL(opcode &code)
{
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "call " << 999;
    code.assembly = ss.str();
}

// Clear bit in I/O register.
void CBI(opcode &code)
{
    programCounter++;
}

// Clear carry flag.
void CLC(opcode &code)
{
    programCounter++;
}

// Load immediate.
void LDI(opcode &code)
{
    // Extract value to be loaded into register.
    short toLoad = ((code.getBits() & 0b0000111100000000) >> 4) | (code.getBits() & 0b0000000000001111);

    // Identify register.
    short reg = ((code.getBits() & 0b0000000011110000) >> 4) + 16;

    // Load stuff into register.
    registers[reg].setValue(toLoad);    

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "ldi  " << registers[reg].getName() << ", " << toLoad;
    code.assembly = ss.str();
}

// Logical Shift Right.
void LSR(opcode &code)
{
    // Parse opcode.
    short toShift = ((code.getBits() & 0b0000000111110000) >> 4);

    // Carry is set if least significant bit of register is set before shift.
    flags.setC(registers[toShift].getNthBit(0));

    // Negative flag
    flags.setN(false);

    // Shift bits in register.
    short shifted = registers[toShift].getValue() >> 1 & 0b0111111111111111;
    registers[toShift].setValue(shifted);

    // V

    // Zero flag (determined after shift).
    flags.setZ(registers[toShift].getValue() == 0);

    // S - Sign flag.
    flags.setN(flags.getV() != flags.getN());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "lsr " << registers[toShift].getName();
    code.assembly = ss.str();
}

// Relative Call to Subroutine
void RCALL(opcode &code)
{
    // Parse opcode.
    int toJump = (code.getBits() & 0b0000011111111111);
    if (code.getNthBit(11)) toJump = ((~toJump & 0b011111111111) + 1)*-1;

    // Update program counter.
    programCounter = programCounter + 1 + toJump;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "rcall " << toJump;
    code.assembly = ss.str();
}

// Return from subroutine.
void RET(opcode &code)
{
    programCounter++;

    code.assembly = std::string("ret");
}

// Relative jump.
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
    ss << "sbrc " << registers[toCheck].getName() << ", " << bitNum;
    code.assembly = ss.str();
}

void SLEEP(opcode &code)
{
    programCounter++;
}