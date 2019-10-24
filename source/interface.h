#pragma once

#include <curses.h>
#include <math.h>   // Used for rounding up.
#include <iomanip>  // What's this again?
#include <stdio.h>  // What's this again?
#include <unistd.h> // What's this again?
#include <locale.h>
#include <sstream>
#include <string>
#include <iostream> // Debug only.
#include <bitset>
#include <vector>

#include <program.h>
#include <registers.h>
#include <stack.h>
#include <opParse.h>
#include <opcode.h>

std::vector<std::string> tabs;
int activeTab = 0;

void drawInstructionRow(int progIndex, int y, int x)
{
    // Set color if drawing the active instruction.
    // ---------------------------------------------------------------
    if (progIndex == programCounter) attrset(COLOR_PAIR(1));
    else attrset(COLOR_PAIR(0));

    // Draw the program index row.
    // ---------------------------------------------------------------
    std::stringstream ss;
    ss << std::hex << progIndex;
    std::string counter = ss.str();
    mvaddstr(y+1,x+1,"00000");
    mvaddstr(y+1,7-counter.length(),counter.c_str());

    // Draw the optcode bits.
    // ---------------------------------------------------------------
    ss.str("");
    ss << std::bitset<16>(program[progIndex].getBits());
    mvaddstr(y+1, x+9, ss.str().c_str());

    // Draw assembly if possible. (Clearing row first).
    // ---------------------------------------------------------------
    mvaddstr(y+1, x+28, "               ");
    mvaddstr(y+1, x+28, program[progIndex].assembly.c_str());

    // Reset colours.
    // ---------------------------------------------------------------
    attrset(COLOR_PAIR(0));
}

void drawInstructions()
{
    // Get size of terminal.
    int winX, winY;
    getmaxyx(stdscr, winY, winX);

    // ================================================================
    // Box around instructions.
    // ================================================================

    // Top & bottom lines.
    for (int i = 0; i != 45; i++)
    {
        mvaddstr(0, i, "═");
        mvaddstr(winY-2, i, "═");
    }

    // Side lines.
    for (int y = 1; y < winY-1; y++)
    {
        mvaddstr(y, 0, "║");
        mvaddstr(y, 8, "│");
        mvaddstr(y, 27, "│");
        mvaddstr(y, 45, "║");
    }

    // Corner pieces.
    mvaddstr(0,0,"╔");
    mvaddstr(0,45,"╗");
    mvaddstr(winY-2,0,"╚");
    mvaddstr(winY-2,45,"╝");

    // Connecting pieces.
    mvaddstr(winY-2,8,"╧");
    mvaddstr(winY-2,27,"╧");
    mvaddstr(0,8,"╤");
    mvaddstr(0,27,"╤");

    // ================================================================
    // Each row of instructions.
    // ================================================================

    // Get amount of instructions above active instruction.
    int instructionsCount = program.size();
    int above = programCounter;
    int below = instructionsCount - programCounter;

    // If there are less instructions than space or there are 
    // less instructions above the active instruction we'll
    // start drawing from the top.
    if ( (winY-3) >= instructionsCount || above <= (winY-3)/2)
    {
        for (int i = 0; i < (winY-3) && i < instructionsCount; i++)
        {
            drawInstructionRow(i, i, 1);
        }
    }

    // If there is more space than instructions under the active 
    // instruction, we'll start drawing from the top.
    else if (below <= (winY-3)/2)
    {
        int startIndex = instructionsCount - (winY-3);
        for (int i = 0; i < (winY-3); i++)
        {
            drawInstructionRow(startIndex + i, i, 1);
        }
        
    }

    // If there is less space than instructions we'll place the 
    // active instruction in the middle.
    else {
        int startIndex = programCounter - ((winY-3)/2);
        for (int i = 0; i < (winY-3); i++)
        {
            drawInstructionRow(startIndex + i, i, 1);
        }       
    }

}

void drawTabRegisters(int base)
{
    // Get size of terminal.
    int winX, winY;
    getmaxyx(stdscr, winY, winX);

    // Determine the space to work with.
    int height = winY - 5;
    int width = winX - base - 4;

    // Draw registers.
    int counter = 0;
    for (int collumn = 0; collumn < ceil(32.0/(float)height); collumn++)
    {
        // Draw border to right.
        for (int y = 0; y <= height && counter < 32; y++)
        {
            mvaddstr(y + 2, base + 23 * (collumn+1), "│");
        }

        // Draw register values.
        for (int y = 1; y <= height && counter < 32; y++)
        {
            std::stringstream ss;
            ss << "r" << counter << " " ;
            if (counter < 10) ss << " ";
            ss << std::bitset<16>(registers[counter].getValue());
            mvaddstr(y+2, base + 2 + (23*collumn), ss.str().c_str());

            // Increment counter.
            counter++;
        }
    
    }
}

void drawTabStack(int base)
{
    int winX, winY;
    getmaxyx(stdscr, winY, winX);

    std::stringstream ss;

    // Create message about elements on the stack.
    ss << "Element on stack: " << stack.size() << " ";
    std::string stackElementsMsg = ss.str();
    ss.str("");

    // Create message about the size of the stack.
    ss << "Size of stack: " << (float)stack.size() * 0.008 << " kilobytes";
    std::string stackSizeMsg = ss.str();
    ss.str("");

    // Place messages and line.
    for (int i = 2; i < winX-base-2; i++) mvaddstr(4, base+i, "-");
    mvaddstr(3, base+2, stackElementsMsg.c_str());
    mvaddstr(3, base+2+stackElementsMsg.length(), "│");
    mvaddstr(4, base+2+stackElementsMsg.length(), "0");
    mvaddstr(3, base+2+stackElementsMsg.length()+2, stackSizeMsg.c_str());


    stack.push_back(0);
    stack.push_back(6);
    stack.push_back(9);
    stack.push_back(12);
    stack.push_back(8);
    int counter = 0;
    for (short layer : stack)
    {
        std::stringstream ss;
        ss << std::bitset<8>(layer);
        mvaddstr(5+counter, base+2, ss.str().c_str());
        counter++;
    }
}

void drawTabs()
{
    // Get size of terminal.
    // --------------------------------------------------------
    int winX, winY;
    getmaxyx(stdscr, winY, winX);


    // Determine base offset
    // --------------------------------------------------------
    int base = 47;


    // Draw contents of tab window.
    // --------------------------------------------------------
    switch (activeTab)
    {
        case 0:
            drawTabRegisters(base);
            break;

        case 1:
            drawTabStack(base);
            break;
        
        // Default should never be reached.
        default: break;
    }


    // Draw border arround tab stuff.
    // --------------------------------------------------------
    for (int x = 0; x < (winX-base); x++)
    {
        mvaddstr(2, base + x, "═");
        mvaddstr(winY-2, base + x, "═");
        
    }
    for (int y = 2; y < winY-2; y++)
    {
        mvaddstr(y, base, "║");
        mvaddstr(y, winX-1, "║");
    }
    mvaddstr(winY-2, base, "╚");
    mvaddstr(winY-2, winX-1, "╝");
    mvaddstr(2, winX-1, "╗");


    // Draw tabs.
    // --------------------------------------------------------
    for (int i = 0; i < tabs.size(); i++)
    {
        std::string tab = tabs[i];

        // Things to do for EVERY tab
        // ----------------------------------------------------
        mvaddstr(1,base, "║");
        mvaddstr(1,base+2, tab.c_str());
        mvaddstr(1,base+3+tab.length(), "║");
        for (int a = 0; a < tab.length()+2; a++) {mvaddstr(0,1+base+a, "═");}
        if (i == 0) {mvaddstr(0,base, "╔");}
        if (i+1 != activeTab) mvaddstr(0,base+3+tab.length(), "╗");
        if (i == 0 && i != activeTab) mvaddstr(2,base,"╔");


        // Things to do for the ACTIVE tab
        // ----------------------------------------------------
        if (i == activeTab)
        {
            // Fix corner pieces.
            mvaddstr(0,base, "╔");
            mvaddstr(2,base+3+tab.length(), "╚");
            if (i == 0) mvaddstr(2,base, "║");
            else mvaddstr(2,base, "╝");

            // Overwrite border.
            for (int a = 0; a < tab.length()+2; a++) {mvaddstr(2,1+base+a, " ");}
        }

        // Increase base offset.
        // ----------------------------------------------------
        base = base + tab.length() + 3;
    }
}

void msgBox(std::string msg)
{
    clear();

    // Get size of terminal.
    int winX, winY;
    getmaxyx(stdscr, winY, winX);

    // Calculate box size.
    int boxLength = msg.length() + 4;

    int Xstart = (winX - boxLength) / 2;
    int Ystart = (winY - 3) / 2;

    // Draw message.
    mvaddstr(Ystart+1,Xstart+2,msg.c_str());

    // Characters for interface.
    const char* horizontal = "═";
    const char* vertical = "║";

    // Draw horizontal lines of box.
    for (int i = 0; i < boxLength; i++)
    {
        mvaddch(Ystart-1, Xstart+i, ' ');
        mvaddstr(Ystart, Xstart+i, horizontal);
        mvaddstr(Ystart+2, Xstart+i, horizontal);
        mvaddch(Ystart+3, Xstart+i, ' ');
    }

    // Draw vertical lines of box.
    mvaddstr(Ystart, Xstart, "╔");
    mvaddstr(Ystart+1, Xstart, "║");
    mvaddstr(Ystart+2, Xstart, "╚");
    mvaddstr(Ystart, Xstart+boxLength-1, "╗");
    mvaddstr(Ystart+1, Xstart+boxLength-1, "║");
    mvaddstr(Ystart+2, Xstart+boxLength-1, "╝");

    refresh();
}

void winUpdate()
{
    // Get size of terminal.
    int winX, winY;
    getmaxyx(stdscr, winY, winX);

    // Draw/update all parts of the interface.
    drawInstructions();
    drawTabs();

    // Place helpful guides at the bottom.
    mvaddstr(winY-1,0,"[ENTER] next instruction  ║  [R] reset  ║  [TAB] next tab  ║  [UP/DOWN] scroll  ║  [Ctrl+C] exit");

    // Move cursor to bottom.
    mvaddstr(winY-1,winX-1," ");

    // Refresh terminal.
    refresh();
}

void GUIrun()
{    
    // Start ncurses and determine terminal size.
    // ------------------------------------------------------
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    setlocale(LC_ALL, "");
    keypad(stdscr, true);

    // Setup terminal colours for ncurses.
    // ------------------------------------------------------
    use_default_colors();
    start_color();
    init_pair(0, 7, 0);
    init_pair(1, 7, 6);

    // Create/name all tabs
    // ------------------------------------------------------
    tabs.push_back("registers");
    tabs.push_back("stack");
    tabs.push_back("flags");
    tabs.push_back("memory");
    tabs.push_back("I/O");

    // Do intial screen update to draw interface for the first time.
    // ------------------------------------------------------
    winUpdate();

    bool running = true;
    int key;
    void(* func)(opcode &code);

    while (running)
    {
        // Check if program has run out.
        // ----------------------------------------
        if (programCounter == program.size())
        {
            msgBox("The program has finished.");
            exit(0);
        }

        // Input parsing.
        // ----------------------------------------
        key = getch();
        switch (key)
        {   
            case KEY_RESIZE: 
                clear();
                winUpdate();
                break;

            case '\n': // KEY_ENTER
                func = parseOpcode(program[programCounter], &SearchTree);
                func(program[programCounter]);
                winUpdate();
                break;
            
            case 'r': // RESET
                programCounter = 0;
                resetRegisters();
                winUpdate();
                break;

            case KEY_RIGHT:
            case '\t': // OR KEY_TAB
                if (activeTab < tabs.size()-1) activeTab++;
                else activeTab = 0;

                clear();
                winUpdate();
                break;
            
            case KEY_LEFT:
                if (activeTab > 0) activeTab--;
                else activeTab = tabs.size()-1;

                clear();
                winUpdate();
                break;

            case KEY_UP:
                // running = false;
                break;

            case KEY_DOWN:
                // running = false;
                break;

            default:
                break;
        }
    }
    
    // Cleanup
    // ------------------------------------------------------
    keypad(stdscr, false);
    endwin();
}