#pragma once

#include <string>

struct opcode
{
    // Raw bytes of optcode.
    int mainbytes;
    int extrabytes;
    bool is32bit = false;
    std::string assembly;

    // Constructor.
    opcode(int code) {this->mainbytes = code & 0b01111111111111111;}

    // Get nth bit function.
    int getNthBit(int n) {return this->mainbytes >> n & 0b01;}
    int getBits() {return this->mainbytes;}

    // Support for 32 bit instructions.
    void make32bit(int newBytes) {this->extrabytes = newBytes; is32bit = true;}
};
