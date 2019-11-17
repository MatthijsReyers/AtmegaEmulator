#pragma once

#include <opcode.h>
#include <instructions.h>

struct node
{
    void(* instruct)(opcode &code) = nullptr;
    struct node *zero = nullptr;
    struct node *one = nullptr;
};


// Main parrent node of the tree.
// ============================================
node SearchTree;


// Function for searching in the tree.
// ============================================
auto parseOpcode(opcode code, node *currentRoot)
{
    // Return instruction if we've found it.
    if ((*currentRoot).instruct) return (*currentRoot).instruct;

    // Case current bit == 1.
    else if ((code.getBits() >> 15) & 0b01 == 1)
    {
        if ((*currentRoot).one) return parseOpcode((code.getBits() << 1), (*currentRoot).one);
        else throw "The provided hex file contains an instruction not supported by this program.";
    }

    // Case current bit == 0.
    else {
        if ((*currentRoot).zero) return parseOpcode((code.getBits() << 1), (*currentRoot).zero);
        else throw "The provided hex file contains an instruction not supported by this program.";
    }
}



// Special function for identifying instructions that
// don't work in the search tree.
// ===================================================
void SPECIAL(opcode &code)
{
    // Deal with break statement.
    if (code.getBits() == 0b1001010110011000) BREAK(code);
    
    // Deal with 32 bit instructions.
    else if (code.is32bit)
    {
        CALL(code);
    }

    else // Deal with 16 bit instructions.
    {
        if ((code.getBits() & 0b1111111000001111) == 0b1001010000000101) ASR(code);
        else if ((code.getBits() & 0b1111111000001111) == 0b1001010000000110) LSR(code);
        else if ((code.getBits() & 0b1111111110001111) == 0b1001010010001000) BCLR(code);
        else if ((code.getBits() & 0b1111111110001111) == 0b1001010000001000) BSET(code);
    }    
}


// Functions used for constructing the tree.
// ============================================
void addopcode(void(& instruct)(opcode &code), int opcode, int codeLength)
{
    node *latest;
    latest = &SearchTree;
    for (int i = 0; i <= codeLength; i++)
    {
        // Add function pointer to struct if we hit the bottom.
        if (i == codeLength)
        {
            if ((*latest).instruct) throw "An optcode is already in the tree at this location.";
            else (*latest).instruct = &instruct;
        }

        // Case 1
        else if (opcode >> (codeLength - 1 - i) & 0b01)
        {
            // Create 1 node if it does not exist.
            if (!(*latest).one) {
                node *newNode = new node();
                (*latest).one = newNode;
            }
            latest = (*latest).one;
        }

        // Case 0
        else {
            // Create 0 node if it does not exist.
            if (!(*latest).zero) {
                node *newNode = new node();
                (*latest).zero = newNode;
            }
            latest = (*latest).zero;
        }
    }
}

void initSearchTree()
{
    addopcode(SPECIAL, 0b1001010, 7);

    addopcode(ADC, 0b000111, 6);
    addopcode(ADD, 0b000011, 6);
    addopcode(ADIW,0b10010110, 8);
    addopcode(AND, 0b001000, 6);
    addopcode(ANDI,0b0111, 4);

    addopcode(BLD, 0b1111100, 7);
    addopcode(BRBC,0b111101, 6);
    addopcode(BRBS,0b111100, 6);
    addopcode(BST, 0b1111101, 7);

    addopcode(LDI, 0b1110, 4);
    addopcode(SBRC,0b1111110, 7);
    addopcode(RJMP,0b1100, 4);
}




