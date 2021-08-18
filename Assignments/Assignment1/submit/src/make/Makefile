# src folder
SRC = ../
# output folder
OUT = ../../out/

# compiler
CC = cc

# compiler flags
FLAGS = -B -c
EXEFLAGS = $(FLAGS) $(FILES)
DEBUGGERFLAGS = -n $(FLAGS) $(FILES)

FILES = $(SRC)main.c $(SRC)functions.c

# outputs
exe: $(FILES)
	$(CC) $(EXEFLAGS) -o "$(OUT)shell.exe"

debug: $(CC) debug.o -o "$(OUT)debug"

debug.o:  $(FILES)
	$(CC) $(DEBUGGERFLAGS)
