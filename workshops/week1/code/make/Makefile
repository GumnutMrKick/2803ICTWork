# src folder
SRC = ../src/
# output folder
OUT = ../out/

# compiler
CC = cc

# compiler flags
FLAGS = -B -c
EXEFLAGS = $(FLAGS) $(FILES)
DEBUGGERFLAGS = -n $(FLAGS) $(FILES)

FILES = $(SRC)main.c $(SRC)leading.c $(SRC)adjust.c $(SRC)trailing.c

# outputs
exe: $(FILES)
	$(CC) $(EXEFLAGS) -o "$(OUT)foo.exe"

debug: $(CC) debug.o -o "$(OUT)debug"

debug.o:  $(FILES)
	$(CC) $(DEBUGGERFLAGS)
