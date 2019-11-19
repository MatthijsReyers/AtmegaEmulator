#pragma once

#include <registers.h>

#include <vector>
#include <string>

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
        auto push(int item, std::string name);
        auto pop();

        // Items on stack options.
        int getItems() {return this->items;}
        void setItems(int i) {this->items = i;}
};

void stackObj::updateReg()
{
    registers[0x05D].setValue(this->stackpointer & 0b00000000011111111);
    registers[0x05E].setValue((this->stackpointer & 0b01111111100000000) >> 8);
}

auto stackObj::push(int item, std::string name)
{
    this->decPointer();
    this->items++;
    registers[this->stackpointer].setValue(item);
    registers[this->stackpointer].setName(name);
    return this->stackpointer;
}

void stackObj::incPointer()
{
    this->stackpointer++;
    updateReg();
}

void stackObj::decPointer()
{
    this->stackpointer--;
    updateReg();
}

auto stackObj::pop()
{
    registers[this->stackpointer].setValue(0);
    registers[this->stackpointer].setName("empty   ");
    this->incPointer();
    this->items--;
    int temp = registers[this->stackpointer].getValue();
    return temp;
}

//
// -------------------------------------------------------------------
stackObj stack;
void initStack()
{
    stack.setPointer(registers.size()+1);
}

void resetStack()
{
    stack.setPointer(registers.size()+1);
    stack.setItems(0);

    for (int i = 0x0100; i <= registers.size(); i++)
    {
        registers[i].setName("empty   ");
    }
}