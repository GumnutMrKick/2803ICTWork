# src folder
SRC = ../
# output folder
OUT = ../../out/

# compiler
CC = gcc

# compiler flags
FLAGS = -B -c

SHARED = $(SRC)shared_includes.c

SERVERFILES = $(SRC)game_server.c $(SHARED)
SERVERFLAGS = $(FLAGS) $(SERVERFILES) -o "$(OUT)game_server.exe"

CLIENTFILES = $(SRC)game_client.c $(SHARED)
CLIENTFLAGS = $(FLAGS) $(CLIENTFILES) -o "$(OUT)game_client.exe"

# outputs
all: client server

client: $(CLIENTFILES)
	$(CC) $(CLIENTFLAGS)

server:$(SERVERFILES)
	$(CC) $(SERVERFLAGS)