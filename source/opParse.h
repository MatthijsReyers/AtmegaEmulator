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
        else throw "this instruction does not exist";
    }

    // Case current bit == 0.
    else {
        if ((*currentRoot).zero) return parseOpcode((code.getBits() << 1), (*currentRoot).zero);
        else throw "this instruction does not exist";
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
    try {
        addopcode(ADC, 0b000111, 6);
        addopcode(ADD, 0b000011, 6);
        addopcode(AND, 0b001000, 6);
        addopcode(ANDI,0b0001, 4);
        addopcode(ASR, 0b1001010, 4);
        addopcode(LDI, 0b1110, 4);
        addopcode(SBRC,0b1111110, 7);
        // addopcode(LSR, 0b1001010, 7);
    }
    catch (const char* err) {std::cout << "ERROR: " << err << "\n"; exit(1);}
}
