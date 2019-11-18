#pragma once

#include <registers.h>

#include <vector>

class stackObj
{
    private:
        int stackpointer;
        int items;
        void updateReg();

    public:
        // Pointer related functions.
        void incPointer();
        void decPointer();
        auto setPointer(int p) {this->stackpointer = p; updateReg();}
        auto getPointer() {return this->stackpointer;}

        // Stack related functions.
        auto push(int item);
        auto pop();

        // Items on stack options.
        int getItems() {return this->items;}
};

void stackObj::updateReg()
{
    registers[0x05D].setValue(this->stackpointer & 0b00000000011111111);
    registers[0x05E].setValue(this->stackpointer & 0b01111111100000000);
}

auto stackObj::push(int item)
{
    this->incPointer();
    registers[this->stackpointer].setValue(item);
    registers[this->stackpointer].setName("STACK");
    return this->stackpointer;
}

void stackObj::incPointer()
{
    this->stackpointer++;
    this->items++;
    updateReg();
}

void stackObj::decPointer()
{
    if (this->items > 0) {
        this->stackpointer--;
        this->items--;
        updateReg();
    }
    else throw "Wow, don't underflow the stack now...";
}

auto stackObj::pop()
{
    int temp = registers[this->stackpointer].getValue();
    registers[this->stackpointer].setValue(0);
    registers[this->stackpointer].setName("reserved");
    this->decPointer();
    return temp;
}

//
// -------------------------------------------------------------------
stackObj stack;
void initStack()
{
    stack.setPointer(registers.size() - 1);
}
void resetStack()
{
    for (int i = 0; i < stack.getItems(); i++) stack.pop();
}