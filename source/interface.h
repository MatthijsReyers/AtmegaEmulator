#pragma once

#include <curses.h>
#include <locale.h>
#include <math.h>   // Used for rounding up.
#include <string>
#include <sstream>
#include <bitset>
#include <vector>

#include <program.h>
#include <registers.h>
#include <flags.h>
#include <stack.h>
#include <opParse.h>
#include <opcode.h>

class GUI
{
    private:
        bool running = true;
        int key;
        void(* func)(opcode &code);

        std::vector<std::string> tabs;
        int activeTab = 0;
        int winX, winY;

        void drawHozLine(int y, int x, int length)
        {
            for (int i = 0; i < length; i++) mvaddstr(y, x+i, "═");
        }

        void drawVerLine(int y, int x, int length)
        {
            for (int i = 0; i < length; i++) mvaddstr(y+i, x, "║");
        }

        void drawVerLineThin(int y, int x, int length)
        {
            for (int i = 0; i < length; i++) mvaddstr(y+i, x, "│");
        }

        void drawBox(int y, int x, int width, int height)
        {
            // Horizontal lines.
            for (int i = 0; i < width; i++)
            {
                mvaddstr(y, x+i, "═");
                mvaddstr(y+height, x+i, "═");
            }

            // Vertical lines.
            for (int i = 0; i < height; i++)
            {
                mvaddstr(y+i, x, "║");
                mvaddstr(y+i, x+width-1, "║");
            }

            // Corner pieces.
            mvaddstr(y,x,"╔");
            mvaddstr(y,x+width-1,"╗");
            mvaddstr(y+height,x,"╚");
            mvaddstr(y+height,x+width-1,"╝");
        }

        void drawInstructionRow(int progIndex, int y, int x)
        {
            // Set color if drawing the active instruction.
            if (progIndex == programCounter) attrset(COLOR_PAIR(1));
            else attrset(COLOR_PAIR(0));

            // Draw the program index row.
            std::stringstream ss;
            ss << std::hex << progIndex;
            std::string counter = ss.str();
            mvaddstr(y+1,x+1,"00000");
            mvaddstr(y+1,7-counter.length(),counter.c_str());

            // Draw the optcode bits.
            ss.str("");
            ss << std::bitset<16>(program[progIndex].getBits());
            mvaddstr(y+1, x+9, ss.str().c_str());

            // Draw assembly if possible. (Clearing row first).
            mvaddstr(y+1, x+28, "               ");
            mvaddstr(y+1, x+28, program[progIndex].assembly.c_str());

            // Reset colours.
            attrset(COLOR_PAIR(0));
        }

        void drawInstructions()
        {
            // Box around instructions.
            drawBox(0,0,46,winY-2);

            // Lines between instructions.
            drawVerLineThin(1,8,winY-3);
            drawVerLineThin(1,27,winY-3);

            // Get temp vars needed for logic below.
            int instructionsCount = program.size();
            int above = programCounter;
            int below = instructionsCount - programCounter;

            // If there are less instructions than space or there are 
            // less instructions above the active instruction we'll
            // start drawing from the top.
            if ( (winY-3) >= instructionsCount || above <= (winY-3)/2)
            {
                for (int i = 0; i < (winY-3) && i < instructionsCount; i++)
                    drawInstructionRow(i, i, 1);
            }

            // If there is more space than instructions under the active 
            // instruction, we'll start drawing from the top.
            else if (below <= (winY-3)/2)
            {
                int startIndex = instructionsCount - (winY-3);
                for (int i = 0; i < (winY-3); i++)
                    drawInstructionRow(startIndex + i, i, 1);
            }

            // If there is less space than instructions we'll place the 
            // active instruction in the middle.
            else 
            {
                int startIndex = programCounter - ((winY-3)/2);
                for (int i = 0; i < (winY-3); i++)
                    drawInstructionRow(startIndex + i, i, 1);
            }

        }

        void drawFlags()
        {
            // Outer box.
            drawBox(2,winX-16,16,winY-4);

            // Make it pretty.
            mvaddstr(3,winX-14,"Status flags");
            mvaddstr(4,winX-14,"───────┬────");
            drawVerLineThin(5,winX-7,8);
            mvaddstr(13,winX-14,"───────┴────");

            // Draw flag names.
            mvaddstr(5, winX-14,"C Flag");
            mvaddstr(6, winX-14,"Z Flag");
            mvaddstr(7, winX-14,"N Flag");
            mvaddstr(8, winX-14,"V Flag");
            mvaddstr(9, winX-14,"S Flag");
            mvaddstr(10,winX-14,"H Flag");
            mvaddstr(11,winX-14,"T Flag");
            mvaddstr(12,winX-14,"I Flag");

            // Draw flag states.
            for (int i = 0; i < 8; i++)
            {
                bool state = registers[0x5F].getNthBit(i);
                if (state) mvaddstr(5+i,winX-5, "SET");
                else {
                    attrset(COLOR_PAIR(2));
                    mvaddstr(5+i,winX-5, "CLS");
                    attrset(COLOR_PAIR(0));
                }
            }
        }

        void drawTabBasicView(int base, int end)
        {
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

        void drawTabSRAM(int base, int end)
        {
            // General purpose stringstream.
            std::stringstream ss;

            // Create message about size of sram.
            ss << "SRAM size: " << (registers.size() * 0.016) << " kilobytes ";
            std::string sramSizeMsg = ss.str();
            ss.str("");

            // Create message about size of extended sram.
            ss << "Internal SRAM size: " << (registers.size()-0x0100) * 0.016 << " kilobytes ";
            std::string interSizeMsg = ss.str();
            ss.str("");

            // Place messages and line.
            drawHozLine(4,base,end-base-2);
            mvaddstr(3, base+2, sramSizeMsg.c_str());
            mvaddstr(3, base+2+sramSizeMsg.length(), "║ ");
            mvaddstr(4, base+2+sramSizeMsg.length(), "╩");
            mvaddstr(3, base+2+sramSizeMsg.length()+2, interSizeMsg.c_str());

            // Figure out how many collumns/rows to draw.
            int collumns = (end - base - 1) / 32;
            int rows = winY - 9 ;

            // Draw every collumn.
            int address = 0;
            for (int c = 0; c < collumns; c++)
            {
                // Quick fix.
                if (c != collumns - 1) mvaddstr(4, base+32, "╦");

                // Draw all vertical lines.
                drawVerLineThin(5, base+10, winY-8);
                drawVerLineThin(5, base+21, winY-8);
                if (c != collumns - 1) drawVerLine(5, base+32, winY-8);
                
                // Draw table header.
                mvaddstr(5, base+2, "Address │ Reg name │ Contents");
                mvaddstr(6, base+2, "────────┼──────────┼─────────");

                // Draw all the SRAM contents.
                for (int i = 0; i < rows && address < registers.size(); i++)
                {
                    // Base address for every row.
                    attron(COLOR_PAIR(2));
                    mvaddstr(7+i, base+2, "0x00000");
                    attroff(COLOR_PAIR(2));

                    // Memmory address.
                    ss.str(""); ss << std::hex << address;
                    mvaddstr(7+i, base+9-ss.str().length(), ss.str().c_str());

                    // Row name.
                    if (registers[address].getName() == "reserved") attron(COLOR_PAIR(2));
                    else if (registers[address].getName() == "empty   ") attron(COLOR_PAIR(2));
                    mvaddstr(7+i, base+12, registers[address].getName().c_str());
                    attroff(COLOR_PAIR(2));

                    // Row contents.
                    ss.str(""); ss << std::bitset<8>(registers[address].getValue());
                    mvaddstr(7+i, base+23, ss.str().c_str());

                    // Increment address.
                    address++;
                }

                // Increase base offset for next collumn.
                base = base + 32;

                // Check if we really need more collumns.
                if (address >= registers.size()) break;// c = collumns;
            }
        }

        void drawTabStack(int base, int end)
        {
            // General purpose stringstream.
            std::stringstream ss;

            // Create message about elements on the stack.
            ss << "Items on stack: " << stack.getItems() << "/" << (registers.size() - 0x0100) << " ";
            std::string stackElementsMsg = ss.str();
            ss.str("");

            // Create message about the size of the stack.
            ss << "Stack size: " << (float)(stack.getItems()) * 0.016 << " kilobytes ";
            std::string stackSizeMsg = ss.str();
            ss.str("");

            // Create message about stack pointer.
            ss << "Pointer addr: 0x0" << std::hex << stack.getPointer() << "      ";
            std::string stackPointMsg = ss.str();
            ss.str("");

            // Place messages and line.
            drawHozLine(4,base,end-base-2);
            mvaddstr(3, base+2, stackSizeMsg.c_str());
            mvaddstr(3, base+2+stackSizeMsg.length(), "║ ");
            mvaddstr(4, base+2+stackSizeMsg.length(), "╩");
            mvaddstr(3, base+2+stackSizeMsg.length()+2, stackElementsMsg.c_str());
            mvaddstr(3, base+2+stackSizeMsg.length()+2+stackElementsMsg.length(), "║ ");
            mvaddstr(4, base+2+stackSizeMsg.length()+2+stackElementsMsg.length(), "╩");
            mvaddstr(3, base+2+stackSizeMsg.length()+2+stackElementsMsg.length()+2, "                      ");
            mvaddstr(3, base+2+stackSizeMsg.length()+2+stackElementsMsg.length()+2, stackPointMsg.c_str());

            // Figure out how many collumns/rows to draw.
            int collumns = (end - base - 1) / 32;
            int rows = winY - 9;

            // Draw every collumn.
            int address = registers.size();
            for (int c = 0; c < collumns; c++)
            {
                // Quick fix.
                if (c != collumns - 1) mvaddstr(4, base+32, "╦");

                // Draw all vertical lines.
                for (int y = 0; y < winY-7; y++)
                {
                    mvaddstr(5+y, base+10, "│");
                    mvaddstr(5+y, base+21, "│");
                    if (c != collumns - 1) mvaddstr(5+y, base+32, "║");
                }
                
                // Draw table header.
                mvaddstr(5, base+2, "Address │ Use      │ Contents");
                mvaddstr(6, base+2, "────────┼──────────┼─────────");

                // Draw all the SRAM contents.
                for (int i = 0; i < rows && address > 0x0100; i++)
                {
                    // Base address for every row.
                    attron(COLOR_PAIR(2));
                    mvaddstr(7+i, base+2, "0x00000");
                    attroff(COLOR_PAIR(2));

                    // Memmory address.
                    ss.str(""); ss << std::hex << address;
                    mvaddstr(7+i, base+9-ss.str().length(), ss.str().c_str());

                    // Row name.
                    if (registers[address].getName() == "reserved") attron(COLOR_PAIR(2));
                    if (registers[address].getName() == "empty   ") attron(COLOR_PAIR(2));
                    mvaddstr(7+i, base+12, registers[address].getName().c_str());
                    attroff(COLOR_PAIR(2));

                    // Row contents.
                    ss.str(""); ss << std::bitset<8>(registers[address].getValue());
                    mvaddstr(7+i, base+23, ss.str().c_str());

                    // Increment address.
                    address--;
                }

                // Increase base offset for next collumn.
                base = base + 32;

                // Check if we really need more collumns.
                if (address >= registers.size()) break;// c = collumns;
            }
        }

        void drawTabs()
        {
            // Create/name all tabs.
            // ------------------------------------------------------
            tabs.clear();
            tabs.push_back("Basic view");
            tabs.push_back("SRAM");
            tabs.push_back("Stack");
            tabs.push_back("I/O");
            

            // Determine base and end offset.
            // --------------------------------------------------------
            int base = 47;
            int endoff;
            if (winX - base - 17 > 32)
            {
                endoff = winX - 17;
                drawFlags();
            }
            else endoff = winX;


            // Draw contents of tab window.
            // --------------------------------------------------------
            switch (activeTab)
            {
                // Default should never be reached.
                default:
                case 0: drawTabBasicView(base, endoff); break;
                case 1: drawTabSRAM(base, endoff); break;
                case 2: drawTabStack(base, endoff); break;
            }


            // Draw base box.
            // --------------------------------------------------------
            drawBox(2,base,endoff-base,winY-4);


            // Draw tabs.
            // --------------------------------------------------------
            for (int i = 0; i < tabs.size(); i++)
            {
                // Get name of tab.
                std::string tab = tabs[i];

                // Things to do for EVERY tab
                mvaddstr(1,base, "║");
                mvaddstr(1,base+2, tab.c_str());
                mvaddstr(1,base+3+tab.length(), "║");
                drawHozLine(0,base+1,tab.length()+2);
                if (i == 0) mvaddstr(0,base, "╔");
                if (i+1 != activeTab) mvaddstr(0,base+3+tab.length(), "╗");
                if (i == 0 && i != activeTab) mvaddstr(2,base,"╔");

                // Things to do for the ACTIVE tab
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

        struct msgBox
        {
            std::string msg1 = "";
            std::string msg2 = "";
            std::string msg3 = "";

            int count;

            msgBox(std::string msg1) {this->count = 1; this->msg1 = msg1; drawBox();}
            msgBox(std::string msg1, std::string msg2) {this->count = 2; this->msg1 = msg1; this->msg2 = msg2; drawBox();}
            msgBox(std::string msg1, std::string msg2, std::string msg3) {this->count = 3; this->msg1 = msg1; this->msg2 = msg2; this->msg3 = msg3; drawBox();}

            void drawBox()
            {
                // Get size of terminal.
                int winX, winY;
                getmaxyx(stdscr, winY, winX);

                // Calculate box size.
                int boxLength;
                if (msg1.length() >= msg2.length() && msg1.length() >= msg3.length()) boxLength = msg1.length();
                if (msg2.length() >= msg1.length() && msg2.length() >= msg3.length()) boxLength = msg2.length();
                if (msg3.length() >= msg1.length() && msg3.length() >= msg2.length()) boxLength = msg3.length();
                
                // 
                int Xstart = (winX - boxLength) / 2;
                int Ystart = (winY - 2 - this->count) / 2;

                clear();

                // // Draw messages.
                if (count >= 1) {
                    mvaddstr(Ystart+1, Xstart, msg1.c_str());
                    mvaddstr(Ystart+1, Xstart-2, "║");
                    mvaddstr(Ystart+1, Xstart+boxLength+1, "║");}
                if (count >= 2) {
                    mvaddstr(Ystart+2, Xstart, msg2.c_str());
                    mvaddstr(Ystart+2, Xstart-2, "║");
                    mvaddstr(Ystart+2, Xstart+boxLength+1, "║");}
                if (count >= 3) {
                    mvaddstr(Ystart+3, Xstart, msg3.c_str());
                    mvaddstr(Ystart+2, Xstart-2, "║");
                    mvaddstr(Ystart+3, Xstart+boxLength+1, "║");}
                
                // Characters for interface.
                const char* horizontal = "═";
                const char* vertical = "║";

                // Draw horizontal lines of box.
                for (int i = -1; i != boxLength+1; i++) {
                    mvaddstr(Ystart, Xstart+i, horizontal);
                    mvaddstr(Ystart+count+1, Xstart+i, horizontal);}

                // Place corner pieces.
                mvaddstr(Ystart, Xstart-2, "╔");
                mvaddstr(Ystart+count+1, Xstart-2,"╚");
                mvaddstr(Ystart, Xstart+1+boxLength, "╗");
                mvaddstr(Ystart+count+1, Xstart+1+boxLength,"╝");

                mvaddstr(winY-1,winX-1," ");

                refresh();
            }
        };

        void winUpdate()
        {
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

    public:
        GUI()
        {
            // Start ncurses and determine terminal size.
            setlocale(LC_ALL, "");
            initscr();
            noecho();
            getmaxyx(stdscr, winY, winX);    
            keypad(stdscr, true);

            // Setup terminal colours for ncurses.
            use_default_colors();
            start_color();
            init_pair(0, 0, 0);
            init_pair(1, 0, 7);
            init_pair(2, 8, 0);
        }

        void start()
        {
            // Do intial screen update to draw interface for the first time.
            winUpdate();        

            // User input loop.            
            while (running)
            {
                // Check if program has run out.
                if (programCounter == program.size())
                {
                    while (key != 'q' && key != 'r')
                    {
                        msgBox("      The program has finished.     ","Press [Q] to exit and [R] to restart");
                        key = getch();
                        if (key == 'q') running = false;
                        else if (key == 'r') {
                            programCounter = 0;
                            resetRegisters();
                            resetFlags();
                            resetStack();
                            clear();
                        }
                    }
                }

                // Input parsing.
                // ----------------------------------------
                key = getch();
                switch (key)
                {   
                    case KEY_RESIZE: 
                        clear();
                        getmaxyx(stdscr, winY, winX);
                        break;

                    case '\n': // KEY_ENTER
                        if (programCounter < program.size() && programCounter >= 0)
                        {
                            func = parseOpcode(program[programCounter], &SearchTree);
                            func(program[programCounter]);
                        }
                        else {
                            while (key != 'q' && key != 'r')
                            {
                                msgBox("The program counter points to a place outside the program","          Press [Q] to exit and [R] to restart.          ");
                                key = getch();
                                if (key == 'q') running = false;
                                else if (key == 'r') {
                                    programCounter = 0;
                                    resetRegisters();
                                    resetFlags();
                                    resetStack();
                                    clear();
                                    winUpdate();
                                }
                            }
                        }
                        break;
                    
                    case 'r': // RESET
                        programCounter = 0;
                        resetRegisters();
                        resetFlags();
                        resetStack();
                        break;

                    case KEY_RIGHT:
                    case '\t': // OR KEY_TAB
                        if (activeTab < tabs.size()-1) activeTab++;
                        else activeTab = 0;
                        clear();
                        break;
                    
                    case KEY_LEFT:
                        if (activeTab > 0) activeTab--;
                        else activeTab = tabs.size()-1;

                        clear();
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

                // Update window
                // ----------------------------------------
                winUpdate();
            }
        }

        void stop()
        {
            // Cleanup
            keypad(stdscr, false);
            endwin();
        }
};