#pragma once

#include <opcode.h>
#include <instructions.h>

struct node
{
    void(* instruct)(opcode &code);
    bool hasInst = false;

    struct node *zero;
    bool hasZero = false;

    struct node *one;
    bool hasOne = false;
};


// Main parrent node of the tree.
// ============================================
node SearchTree;


// Function for searching in the tree.
// ============================================
auto parseOpcode(opcode &code)
{
    node *latest;
    latest = &SearchTree;
    for (int i = 0; i < 16; i++)
    {
        // Debug message:
        std::cout << "Level: " << i << std::endl;
        std::cout << "zero:  " << (* latest).hasZero << std::endl;
        std::cout << "one:   " << (* latest).hasOne << std::endl;
        std::cout << "-------------" << std::endl;

        // Check if we have found the function for this opcode.
        if ((*latest).hasInst)
        {
            return (*latest).instruct;
        }

        // Check if next bit is a 1.
        else if (code.getNthBit(16-i) & 0b01 == 1)
        {
            if ((* latest).hasOne) {latest = (* latest).one;}
            else {throw "opcode does not exist.";}
        }

        // Then the next bit must be a 0.
        else if (code.getNthBit(16-i) & 0b01 == 0)
        {
            if ((* latest).hasZero) {latest = (* latest).zero;}
            else {throw "opcode does not exist.";}
        }
    }

    // This code can only be reached by searching for a optcode 
    // that's not in the search tree. 
    throw "opcode does not exist.";
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
            if ((*latest).hasInst == false)
            {
                (*latest).instruct = &instruct;
            }
        }

        // Case 1
        else if (opcode >> (codeLength - 1 - i) & 0b01)
        {
            if ((*latest).hasOne)
            {
                latest = (*latest).one;
            }
            else{
                node newNode;
                (*latest).one = &newNode;
                (*latest).hasOne = true;
                latest = &newNode;
            }
        }
        

        // Case 0
        else {
            if ((*latest).hasZero)
            {
                latest = (*latest).zero;
            }
            else{
                node newNode;
                (*latest).zero = &newNode;
                (*latest).hasZero = true;
                latest = &newNode;
            }
        }
    }
}
void initSearchTree()
{

}


