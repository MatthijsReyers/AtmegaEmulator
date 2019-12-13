
all: cleanupold compile

cleanupold:
		@echo "Cleaning up old files..."
		rm -f atemu
		rm -f atemu.exe

compile:
		@echo "Compiling source..."
		g++ -I ./source -o atemu ./source/main.cpp -lncursesw

