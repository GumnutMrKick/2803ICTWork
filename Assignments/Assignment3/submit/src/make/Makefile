# src folder
SRC = ../
# output folder
OUT = ../../out/

# compiler
CC = gcc

# compiler flags
FLAGS = -lpthread

# objects
SHAREDOBJS = $(SRC)shared_includes.c 
CLIENTOBJS = $(SRC)client.c $(SRC)client_includes.c $(SHAREDOBJS)
SERVEROBJS = $(SRC)server.c $(SRC)server_includes.c $(SHAREDOBJS)

# outputs
all: client server

client:
	$(CC) $(CLIENTOBJS) $(FLAGS) -o "$(OUT)client.exe"

server:
	$(CC) $(SERVEROBJS) $(FLAGS) -o "$(OUT)server.exe"

shared_includes.o:
	$(CC) -c $(FLAGS) $(SRC)shared_includes.c

client.o:
	$(CC) -c $(FLAGS) $(SRC)client.c

client_includes.o:
	$(CC) -c $(FLAGS) $(SRC)client_includes.c

server.o:
	$(CC) -c $(FLAGS) $(SRC)server.c

server_includes.o:
	$(CC) -c $(FLAGS) $(SRC)server_includes.c