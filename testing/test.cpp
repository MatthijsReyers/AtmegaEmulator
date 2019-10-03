#include <iostream>

void ADD(short &code) {std::cout << code << "\n";}
void LDI(short &code) {std::cout << code << "\n";}
void SBRC(short &code){std::cout << code << "\n";}

struct node
{
    void(* instruct)(short &code);
    bool hasInst = false;

    struct node *zero = nullptr;
    struct node *one = nullptr;
};

node SearchTree;

auto parseOpcode(short code, node *currentRoot)
{
    // Return instruction if we've found it.
    if ((*currentRoot).hasInst) return (*currentRoot).instruct;

    // Case current bit == 1.
    else if ((code >> 15) & 0b01 == 1)
    {
        if ((*currentRoot).one) return parseOpcode((code << 1), (*currentRoot).one);
        else throw "this instruction does not exist";
    }
    // Case current bit == 0.
    else {
        if ((*currentRoot).zero) return parseOpcode((code << 1), (*currentRoot).zero);
        else throw "this instruction does not exist";
    }
}

void addopcode(void(& instruct)(short &code), int opcode, int codeLength)
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
                (*latest).hasInst = true;
            }
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


int main()
{
    addopcode(ADD, 0b000011, 6);
    addopcode(LDI, 0b1110, 4);
    addopcode(SBRC, 0b1111110, 7);

    short firstOpcode = 0b1110000000010011;

    void(* instruction)(short &code) = parseOpcode(firstOpcode, &SearchTree);
    instruction(firstOpcode);
    return 0;
}