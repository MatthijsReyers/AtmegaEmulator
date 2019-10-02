#pragma once

#include <string>

struct opcode
{
    // Raw bytes of optcode.
    short mainbytes;
    short extrabytes;
    bool is32bit = false;
    std::string assembly;

    // Constructor.
    opcode(short code) {this->mainbytes = code;}

    // Get nth bit function.
    short getNthBit(int n) {return this->mainbytes >> n & 0b01;}
    short getBits() {return this->mainbytes;}

    // Support for 32 bit instructions.
    void make32bit(short newBytes) {this->extrabytes = newBytes; is32bit = true;}
};
