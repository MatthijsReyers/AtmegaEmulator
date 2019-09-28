
#include <instructions.h>

struct node
{
    struct node *left;
    struct node *right;
    // void(& instruction)(short &optcode);
};

// Main parrent node of the tree.
// ============================================
node SearchTree;


// Functions used for constructing the tree.
// ============================================
void addcode(node &rootNode, void(& instruct)(short &code), int optcode, int codeLength)
{
    node *latest;
    for (int i = 0; i < codeLength; i++)
    {
        
    }
}
void constructTree(node &rootNode)
{

}


// Functions for searching the tree.
// ============================================
auto parseOptCode(optcode &code)
{
    node latest;
    latest = SearchTree;
    for (int i = 0; i < 16; i++)
    {
        if (latest.instruction == NULL)
    }
    
}