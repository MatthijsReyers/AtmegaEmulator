#pragma once

#include <curses.h>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <sstream>
#include <string>
#include <iostream>
#include <bitset>
#include <vector>

#include <registers.h>
#include <program.h>
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

void drawTabs()
{
    // Get size of terminal.
    int winX, winY;
    getmaxyx(stdscr, winY, winX);

    int base = 47;
    for (int i = 0; i < tabs.size(); i++)
    {
        std::string tab = tabs[i];

        mvaddstr(1,base, "|");
        mvaddstr(1,base+2, tab.c_str());
        mvaddstr(1,base+3+tab.length(), "|");

        base = base + tab.length() + 3;
    }
    base = 47;
}

void drawRegisters()
{
    // Get size of terminal.
    int winX, winY;
    getmaxyx(stdscr, winY, winX);

    for (int i = 0; i < 32; i++)
    {
        std::stringstream ss;
        ss << "r" << i << " " ;
        if (i<10) ss << " ";
        ss << std::bitset<16>(registers[i].getValue()) << std::endl;
        mvaddstr(1+i, winX-23, ss.str().c_str());
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

    // Place helpful guides at the bottom.
    mvaddstr(winY-1,0,"[ENTER] next instruction  ║  [R] reset  ║  [TAB] next tab  ║  [UP/DOWN] scroll  ║  [Ctrl+C] exit");

    // Draw/update all parts of the interface.
    drawInstructions();
    drawRegisters();
    drawTabs();

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
    tabs.push_back("test 01");
    tabs.push_back("test 02");

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

            case 't': // KEY_TAB
                if (activeTab < tabs.size()-1) activeTab++;
                else activeTab = 0;

                clear();
                winUpdate();
                break;

            case KEY_UP:
                running = false;
                break;

            case KEY_DOWN:
                running = false;
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