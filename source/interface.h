#include <curses.h>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <sstream>
#include <string>

#include <iostream>
#include <bitset>

#include <program.h>

void drawInstructionRow(int progIndex, int y, int x)
{
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

    // Draw assembly if possible.
    mvaddstr(y+1, x+28, program[progIndex].assembly.c_str());

    // Special layout in case of drawing the active instruction.
    // ---------------------------------------------------------------
    if (progIndex == programCounter)
    {

    }
}

void drawInstructions()
{
    // Get size of terminal.
    int winX, winY;
    getmaxyx(stdscr, winY, winX);

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

void drawOutline()
{
    // Characters for interface.
    const char* horizontal = "═";
    const char* vertical = "║";

    // Get size of terminal.
    int winX, winY;
    getmaxyx(stdscr, winY, winX);

    // Top & bottom lines.
    for (int i = 0; i != winX; i++)
    {
        mvaddstr(0, i, horizontal);
        mvaddstr(winY-2, i, horizontal);
    }

    // Side lines.
    for (int y = 1; y < winY-1; y++)
    {
        mvaddstr(y, 0, vertical);
        mvaddstr(y, winX-1, vertical);
    }

    // Corner pieces.
    mvaddstr(0,0,"╔");
    mvaddstr(0,winX-1,"╗");
    mvaddstr(winY-2,0,"╚");
    mvaddstr(winY-2,winX-1,"╝");

    // Place helpful guides at the bottom.
    mvaddstr(winY-1,0,"[ENTER] next instruction  ║  [A] start/stop  ║  [Ctrl+C] exit");
}

void winResize()
{
    clear();
    drawInstructions();
    drawOutline();
    refresh();
}

void winUpdate()
{
    clear();
    drawInstructions();
    drawOutline();
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

    winResize(); 

    bool running = true;
    while (running)
    {
        int key = getch();
        switch (key)
        {
        case KEY_RESIZE: 
            winResize();
            break;
        
        case KEY_BACKSPACE:
            running = false;
            break;

        case '\n': // KEY_ENTER
            programCounter++;
            winUpdate();
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