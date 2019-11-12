#pragma once

#include <registers.h>


// Register struct/class.
// ===========================================================
class statusRegister
{
    private:
        // reg status = registers[0x3F];
        // reg status = reg("stat");

    public:
        // template bool get<int x> ();

        // Getters.
        bool getC() {return registers[0x5F].getNthBit(0);}   // Carry Flag
        bool getZ() {return registers[0x5F].getNthBit(1);}   // Zero Flag
        bool getN() {return registers[0x5F].getNthBit(2);}   // Negative Flag
        bool getV() {return registers[0x5F].getNthBit(3);}   // Two’s complement overflow indicator
        bool getS() {return registers[0x5F].getNthBit(4);}   // N ⊕ V, For signed tests
        bool getH() {return registers[0x5F].getNthBit(5);}   // Half Carry Flag
        bool getT() {return registers[0x5F].getNthBit(6);}   // Transfer bit used by BLD and BST instructions
        bool getI() {return registers[0x5F].getNthBit(7);}   // Global Interrupt Enable/Disable Flag

        // Setters.
        void setC(int b) {registers[0x5F].setNthBit(0,b);}
        void setZ(int b) {registers[0x5F].setNthBit(1,b);}
        void setN(int b) {registers[0x5F].setNthBit(2,b);}
        void setV(int b) {registers[0x5F].setNthBit(3,b);}
        void setS(int b) {registers[0x5F].setNthBit(4,b);}
        void setH(int b) {registers[0x5F].setNthBit(5,b);}
        void setT(int b) {registers[0x5F].setNthBit(6,b);}
        void setI(int b) {registers[0x5F].setNthBit(7,b);}
};


// Function for resetting the SREG/flags.
// ===========================================================
statusRegister flags;
void resetFlags()
{
    registers[0x5F].setValue(0);
}