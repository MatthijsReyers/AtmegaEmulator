#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <bitset>

#include <opcode.h>
#include <program.h>
#include <registers.h>
#include <stack.h>
#include <flags.h>


void BREAK(opcode &code)
{
    exit(0);
}

// Add with carry.
void ADC(opcode &code) 
{
    // Parse opcode.
    short Rr = ((code.getBits() & 0b0000000000001111) | ((code.getBits() & 0b0000001000000000) >> 4));
    short Rd = ((code.getBits() & 0b0000000111110000) >> 4);

    bool Rd7 = registers[Rd].getNthBit(7);
    bool Rr7 = registers[Rr].getNthBit(7);
    bool Rd3 = registers[Rd].getNthBit(3);
    bool Rr3 = registers[Rr].getNthBit(3);

    // Calculate result and add to register.
    int result = registers[Rd].getValue() + registers[Rr].getValue();
    if (flags.getC()) result++;
    registers[Rd].setValue(result);

    bool R7 = result & 0b010000000;
    bool R3 = result & 0b000001000;

    // H - Half carry. (Set if there was a borrow from bit 3).
    flags.setH( (Rd3&&Rr3) || (Rr3&&(!R3)) || (Rd3&&(!R3)) );

    // V - Overflow flag. (Set if two’s complement overflow resulted from the operation).
    flags.setV( (Rd7&&Rr7&&(!R7)) || ((!Rd7)&&(!Rr7)&&R7) );

    // N - Negative flag is bit 7.
    flags.setN(R7);

    // Z - Zero flag.
    flags.setZ(result == 0);

    // C - Carry flag.
    flags.setC( ((!Rd7)&&Rr7) || (Rr7&&R7) || (R7&&(!Rd7)) );

    // S - Sign flag.
    flags.setS(flags.getV() != flags.getN());
    
    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "adc  " << registers[Rd].getName() << ", " << registers[Rr].getName();
    code.assembly = ss.str();
}

// Add without carry.
void ADD(opcode &code) 
{
    // Parse opcode.
    short Rr = ((code.getBits() & 0b0000000000001111) | ((code.getBits() & 0b0000001000000000) >> 5));
    short Rd = ((code.getBits() & 0b0000000111110000) >> 4);

    bool Rd7 = registers[Rd].getNthBit(7);
    bool Rr7 = registers[Rr].getNthBit(7);
    bool Rd3 = registers[Rd].getNthBit(3);
    bool Rr3 = registers[Rr].getNthBit(3);

    // Calculate result and add to register.
    int result = registers[Rd].getValue() + registers[Rr].getValue();
    registers[Rd].setValue((short)result);

    bool R7 = result & 0b010000000;
    bool R3 = result & 0b000001000;

    // H - Half carry. (Set if there was a borrow from bit 3).
    flags.setH( (Rd3&&Rr3) || (Rr3&&(!R3)) || (Rd3&&(!R3)) );

    // V - Overflow flag. (Set if two’s complement overflow resulted from the operation).
    flags.setV( (Rd7&&Rr7&&(!R7)) || ((!Rd7)&&(!Rr7)&&R7) );

    // N - Negative flag is bit 7.
    flags.setN(R7);

    // Z - Zero flag.
    flags.setZ(result == 0);

    // C - Carry flag.
    flags.setC( (Rd7&&Rr7) || (Rr7&&(!R7)) || (R7&&(!Rd7)) );

    // S - Sign flag.
    flags.setS(flags.getV() != flags.getN());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "add  " << registers[Rd].getName() << ", " << registers[Rr].getName();
    code.assembly = ss.str();
}

// Add immediate to Word.
void ADIW(opcode &code)
{
    short K = (code.getBits() & 0b01111) + ((code.getBits() & 0b0011000000) >> 2);
    short Rdl = ((code.getBits() & 0b00110000) >> 4) + 24;
    short Rdh = Rdl + 1;

    bool Rdh7 = registers[Rdh].getNthBit(7);

    int base = registers[Rdl].getValue() + (registers[Rdh].getValue() << 8);
    int result = base + K;

    bool R15 = registers[Rdh].getNthBit(7);

    // V - Overflow flag. (Set if two’s complement overflow resulted from the operation).
    flags.setV( (!Rdh7) && R15 );

    // N - Negative flag is bit 7 of high register.
    flags.setN(R15);

    // Z - Zero flag.
    flags.setZ(result == 0);

    // C - Carry flag.
    flags.setC( (!R15) && Rdh7 );

    // S - Sign flag.
    flags.setS(flags.getV() != flags.getN());

    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "adiw " << registers[Rdh].getName() << ":" << registers[Rdl].getName() << ", " << K;
    code.assembly = ss.str();
}

// Logical AND.
void AND(opcode &code)
{
    // Parse opcode.
    short Rr = ((code.getBits() & 0b0000000000001111) | ((code.getBits() & 0b0000001000000000) >> 5));
    short Rd = ((code.getBits() & 0b0000000111110000) >> 4);

    // Calcuate result and add to register.
    int result = registers[Rd].getValue() & registers[Rr].getValue();
    registers[Rd].setValue(result);

    // V (cleared)
    flags.setV(0);

    // N - Negative flag.
    flags.setN(registers[Rd].getNthBit(7));

    // Z - Zeroflag
    flags.setZ(result == 0);

    // S - Sign flag.
    flags.setS(flags.getV() != flags.getN());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "and  " << registers[Rd].getName() << ", " << registers[Rr].getName() << std::endl;
    code.assembly = ss.str();
}

// Logical AND with Immediate.
void ANDI(opcode &code)
{
    // Parse opcode.
    short K = ((code.getBits() & 0b001111) | ((code.getBits() & 0b0000111100000000) >> 4));
    short Rd = 16 + ((code.getBits() & 0b0011110000) >> 4);

    // Calcuate result and add to register.
    int result = registers[Rd].getValue() & K;
    registers[Rd].setValue(result);

    // V - Overflow flag (cleared).
    flags.setV(0);

    // N - Negative flag is bit 7.
    flags.setN(registers[Rd].getNthBit(7));

    // Z - Zero flag.
    flags.setZ(result == 0);

    // S - Sign flag.
    flags.setS(flags.getV() != flags.getN());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "andi " << registers[Rd].getName() << ", " << K;
    code.assembly = ss.str();
}

// Arithmetic Shift Right.
void ASR(opcode &code)
{
    // Parse opcode.
    short Rd = (code.getBits() & 0b0000000111110000) >> 4;

    // Carry flag is set to Bit 0.
    flags.setC(registers[Rd].getNthBit(0));

    // Bit 7 stays bit 7.
    short bit7 = registers[Rd].getNthBit(7);

    // Actually shift bits and add bit 7.
    short result = (registers[Rd].getValue() >> 1) | (bit7 << 7);
    registers[Rd].setValue(result);

    // N - Negative flag is bit 7.
    flags.setN(registers[Rd].getNthBit(7));

    // Z - Zero flag.
    flags.setZ(result == 0);

    // S - Sign flag.
    flags.setS(flags.getV() != flags.getN());
    
    // V - Overflow flag. N ⊕ C (For N and C after the shift)
    flags.setV(flags.getN() != flags.getC());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "asr  " << registers[Rd].getName();
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
    switch (flagNum)
    {
        case 0: ss << "clc"; break;
        case 1: ss << "clz"; break;
        case 2: ss << "cln"; break;
        case 3: ss << "clv"; break;
        case 4: ss << "cls"; break;
        case 5: ss << "clh"; break;
        case 6: ss << "clt"; break;
        case 7: ss << "cli"; break;

        default: ss << "bclr " << flagNum; break;
    }
    code.assembly = ss.str();
}

// Bit load from the T flag in SREG to a bit in register.
void BLD(opcode &code)
{
    // Parse opcode.
    short Rd = (code.getBits() & 0b00111110000) >> 4;
    short bit = (code.getBits() & 0b0111);

    // Update value in register.
    registers[Rd].setNthBit(bit, flags.getT());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "bld  " << Rd << ", " << bit;
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
    short bit = ((code.getBits() & 0b0000000001110000) >> 4);

    // Change the bit in SREG
    registers[0x5F].setNthBit(bit,1);

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    switch (bit)
    {
        case 0: ss << "sec"; break;
        case 1: ss << "sez"; break;
        case 2: ss << "sen"; break;
        case 3: ss << "sev"; break;
        case 4: ss << "ses"; break;
        case 5: ss << "seh"; break;
        case 6: ss << "set"; break;
        case 7: ss << "sei"; break;
        default: ss << "bset " << bit; break;
    }
    code.assembly = ss.str();
}

// Bit store from bit in Register to T flag.
void BST(opcode &code)
{
    short Rd = (code.getBits() & 0b0111110000) >> 4;
    short bit = code.getBits() & 0b0111;

    flags.setT(registers[Rd].getNthBit(bit));


    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "bst  " << registers[Rd].getName() << ", " << bit;
    code.assembly = ss.str();
}

// Clear bit in I/O register.
void CBI(opcode &code)
{
    short A = 0x20 + ((code.getBits() & 0b011111000) >> 3);
    short bit = code.getBits() & 0b0111;

    registers[A].setNthBit(bit, 0);

    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "cbi  " << registers[A].getName() << ", " << bit;
    code.assembly = ss.str();
}

// Compare
void CP(opcode &code)
{
    // Parse opcode
    short Rd = (code.getBits() & 0b0111110000) >> 4;
    short Rr = (code.getBits() & 0b01111) + ((code.getBits() & 0b01000000000) >> 5);

    bool Rd7 = registers[Rd].getNthBit(7);
    bool Rr7 = registers[Rr].getNthBit(7);
    bool Rd3 = registers[Rd].getNthBit(3);
    bool Rr3 = registers[Rr].getNthBit(3);

    int R = registers[Rd].getValue() - registers[Rr].getValue();

    bool R7 = R & 0b010000000;
    bool R3 = R & 0b000001000;

    // H - Half carry. (Set if there was a borrow from bit 3).
    flags.setH( ((!Rd3)&&Rr3) || (Rr3&&R3) || (R3&&(!Rd3)) );

    // V - Overflow flag. (Set if two’s complement overflow resulted from the operation).
    flags.setV( (Rd7&&(!Rr7)&&(!R7)) || ((!Rd7)&&Rr7&&R7) );

    // N - Negative flag is bit 7.
    flags.setN(R7);

    // Z - Zero flag.
    flags.setZ(R == 0);

    // C - Carry flag.
    flags.setC( (Rd7&&Rr7) || (Rr7&&(!R7)) || (R7&&(!Rd7)) );

    // S - Sign flag.
    flags.setS(flags.getV() != flags.getN());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "cp   " << registers[Rd].getName() << ", " << registers[Rr].getName();
    code.assembly = ss.str();
}

// Compare with carry
void CPC(opcode &code)
{
    // Parse opcode
    short Rd = (code.getBits() & 0b0111110000) >> 4;
    short Rr = (code.getBits() & 0b01111) + ((code.getBits() & 0b01000000000) >> 5);

    bool Rd7 = registers[Rd].getNthBit(7);
    bool Rr7 = registers[Rr].getNthBit(7);
    bool Rd3 = registers[Rd].getNthBit(3);
    bool Rr3 = registers[Rr].getNthBit(3);

    int R = registers[Rd].getValue() - registers[Rr].getValue();
    if (flags.getC()) R--;

    bool R7 = R & 0b010000000;
    bool R3 = R & 0b000001000;

    // H - Half carry. (Set if there was a borrow from bit 3).
    flags.setH( ((!Rd3)&&Rr3) || (Rr3&&R3) || (R3&&(!Rd3)) );

    // V - Overflow flag. (Set if two’s complement overflow resulted from the operation).
    flags.setV( (Rd7&&(!Rr7)&&(!R7)) || ((!Rd7)&&Rr7&&R7) );

    // N - Negative flag is bit 7.
    flags.setN(R7);

    // Z - Zero flag.
    flags.setZ(R == 0);

    // C - Carry flag.
    flags.setC( (Rd7&&Rr7) || (Rr7&&(!R7)) || (R7&&(!Rd7)) );

    // S - Sign flag.
    flags.setS(flags.getV() != flags.getN());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "cp   " << registers[Rd].getName() << ", " << registers[Rr].getName();
    code.assembly = ss.str();
}

// Decrement
void DEC(opcode &code)
{
    short Rd = (code.getBits() & 0b000111110000) >> 4;

    // Two’s complement overflow occurs if Rd is $80 before the operation. 
    flags.setV(registers[Rd].getValue() == 0x80);

    short R = registers[Rd].getValue() - 1;
    registers[Rd].setValue(R);

    flags.setN(registers[Rd].getNthBit(7));
    flags.setZ(R == 0);
    flags.setS(flags.getV() != flags.getN());
}

// Exclusive OR
void EOR(opcode &code)
{
    short Rd = (code.getBits() & 0b000111110000) >> 4;
    short Rr = (code.getBits() & 0b01111) + ((code.getBits() & 0b01000000000) >> 9);

    int result = registers[Rd].getValue() ^ registers[Rr].getValue();
    registers[Rd].setValue(result);

    flags.setN(flags.getV() != flags.getN());

    // (Cleared)
    flags.setV(0);

    // MSB of result.
    flags.setN(registers[Rd].getNthBit(7));

    // Result is zero?
    flags.setZ(result == 0);

    std::stringstream ss;
    if (Rd == Rr) ss << "clr  " << registers[Rd].getName();
    else ss << "eor  " << registers[Rd].getName() << ", " << registers[Rr].getName();
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
    short Rd = ((code.getBits() & 0b0000000111110000) >> 4);

    // Carry is set if least significant bit of register is set before shift.
    flags.setC(registers[Rd].getNthBit(0));

    // Negative flag
    flags.setN(false);

    // Shift bits in register.
    short shifted = registers[Rd].getValue() >> 1 & 0b0111111111111111;
    registers[Rd].setValue(shifted);

    // V

    // Zero flag (determined after shift).
    flags.setZ(registers[Rd].getValue() == 0);

    // S - Sign flag.
    flags.setN(flags.getV() != flags.getN());

    // Increment program counter.
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "lsr  " << registers[Rd].getName();
    code.assembly = ss.str();
}

// Copy register
void MOV(opcode &code)
{
    // Parse opcode.
    short Rd = (code.getBits() & 0b000111110000) >> 4;
    short Rr = (code.getBits() & 0b01111) + ((code.getBits() & 0b001000000000) >> 5);

    // Move value.
    registers[Rd].setValue(registers[Rr].getValue());

    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "mov  " << registers[Rd].getName() << ", " << registers[Rr].getName();
    code.assembly = ss.str();
}

// No Operation.
void NOP(opcode &code)
{
    programCounter++;

    // Make a string for translated assembly and put in optcode.
    code.assembly = std::string("nop");
}

// Relative Call to Subroutine
void RCALL(opcode &code)
{
    // Parse opcode.
    int toJump = (code.getBits() & 0b0000011111111111);
    if (code.getNthBit(11)) toJump = ((~toJump & 0b011111111111) + 1)*-1;

    // Push current program counter to stack
    if (stack.getPointer() > 0x0100) {
        int temp = programCounter + 1;
        stack.push(temp & 0b01111111100000000, "retadr H");
        stack.push(temp & 0b00000000011111111, "retadr L");}
    else throw "Stack is smashing into ext I/O registers.";

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
    // Get address from stack.
    int toJump = stack.pop();
    toJump = toJump + (stack.pop() << 8);

    // Update program counter.
    programCounter = toJump;

    // Translate assembly and put in optcode.
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

// Set bit in I/O register.
void SBI(opcode &code)
{
    short A = 0x20 + ((code.getBits() & 0b011111000) >> 3);
    short bit = code.getBits() & 0b0111;

    registers[A].setNthBit(bit, 1);

    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "sbi  " << registers[A].getName() << ", " << bit;
    code.assembly = ss.str();
}

// Skip if Bit in Register is Cleared.
void SBRC(opcode &code)
{
    // Parse opcode
    short bit = ((code.getBits() & 0b0000000000000111));
    short Rr = ((code.getBits() & 0b0000000111110000) >> 4);

    // Get bit from register
    bool state = registers[Rr].getNthBit(bit);

    // Skip instruction if register is clear;
    if (state = false) programCounter = programCounter + 2;
    else programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "sbrc " << registers[Rr].getName() << ", " << bit;
    code.assembly = ss.str();
}

// Substract without carry.
void SUB(opcode &code)
{
    short Rr = (code.getBits() & 0b01111) + ((code.getBits() & 0b01000000000) >> 5);
    short Rd = (code.getBits() & 0b0111110000) >> 4;

    bool Rd7 = registers[Rd].getNthBit(7);
    bool Rr7 = registers[Rr].getNthBit(7);
    bool Rd3 = registers[Rd].getNthBit(3);
    bool Rr3 = registers[Rr].getNthBit(3);

    int result = registers[Rd].getValue() - registers[Rr].getValue();
    registers[Rd].setValue(result);

    bool R7 = result & 0b010000000;
    bool R3 = result & 0b000001000;

    // H - Half carry. (Set if there was a borrow from bit 3; cleared otherwise).
    flags.setH( ((!Rd3)&Rr3) | (Rr3&R3) | (R3&(!Rd3)) );

    // S - Sign flag.
    flags.setN(flags.getV() != flags.getN());

    // V - Overflow flag. (Set if two’s complement overflow resulted from the operation; cleared otherwise.).
    flags.setV( (Rd7&(!Rr7)&R7) | ((!Rd7)&(!Rd7)&R7) );

    // N - Negative flag is bit 7.
    flags.setN(R7);

    // Z - Zero flag.
    flags.setZ(result == 0);

    // C - Carry flag.
    flags.setC( ((!Rd7)&Rr7) | (Rr7&R7) | (R7&(!Rd7)) );

    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "sub  " << registers[Rd].getName() << ", " << registers[Rr].getName();
    code.assembly = ss.str();
}

// Substract without carry.
void SUBI(opcode &code)
{
    short Rd = (code.getBits() & 0b0111110000) >> 4;
    short K = (code.getBits() & 0b01111) + ((code.getBits() & 0b0111100000000) >> 4);

    bool Rd7 = registers[Rd].getNthBit(7);
    bool K7 = 0b010000000 & K;
    bool Rd3 = registers[Rd].getNthBit(3);
    bool K3 = 0b000001000 & K;

    int result = registers[Rd].getValue() - K;
    registers[Rd].setValue(result);

    bool R7 = result & 0b010000000;
    bool R3 = result & 0b000001000;

    // H - Half carry. (Set if there was a borrow from bit 3; cleared otherwise).
    flags.setH( ((!Rd3)&K3) | (K3&R3) | (R3&(!Rd3)) );

    // S - Sign flag.
    flags.setN(flags.getV() != flags.getN());

    // V - Overflow flag. (Set if two’s complement overflow resulted from the operation; cleared otherwise.).
    flags.setV( (Rd7&(!K7)&R7) | ((!Rd7)&(!Rd7)&R7) );

    // N - Negative flag is bit 7.
    flags.setN(R7);

    // Z - Zero flag.
    flags.setZ(result == 0);

    // C - Carry flag.
    flags.setC( ((!Rd7)&K7) | (K7&R7) | (R7&(!Rd7)) );

    programCounter++;

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "subi " << registers[Rd].getName() << ", " << registers[K].getName();
    code.assembly = ss.str();
}

// Swap nibbles.
void SWAP(opcode &code)
{
    // Parse opcode.
    short Rd = (code.getBits() & 0b0111110000) >> 4;

    short hig = (registers[Rd].getValue() & 0b011110000 ) >> 4;
    short low = (registers[Rd].getValue() & 0b000001111 ) << 4;

    registers[Rd].setValue(hig + low);

    // Make a string for translated assembly and put in optcode.
    std::stringstream ss;
    ss << "swap " << Rd;
    code.assembly = ss.str();
}

// ================================ NOT ACTUALLY IMPLEMENTED ================================

void COM(opcode &code)
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

// Set circuit in sleep mode.
void SLEEP(opcode &code)
{
    programCounter++;
    throw "Circuit set in sleep mode";
}
