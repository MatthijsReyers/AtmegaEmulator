#pragma once

struct optcode
{
    // Raw bytes of optcode.
    short fullbits;

    // Constructor.
    optcode(short code) {this->fullbits = code;}

    // Get nth bit function.
    short getNthBit(int n) {return 0b0001 & (this->fullbits >> (15-n));}
    short getBits() {return this->fullbits;}
} ;
