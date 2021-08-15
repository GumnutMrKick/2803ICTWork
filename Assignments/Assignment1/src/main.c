#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "includes.h"
#include "functions.h"

// main program loop
int main(int argc, char* argv[]) {
	
	// running flag
	bool running = true;
	char * command, * in_argv;
	int in_len;

	// introduce program
	printf("Welcome to S5132483's shell program for 2803ICT Assignment 1\n");
	printf("PLEASE NOTE: all input has an arbitrary limit of 1000 characters\n");


	// main shell loop
	while (running) {

		// free the array
		free(command);

		// allocate the input
		command = (char*)calloc(MAXIN, sizeof(char));

		printf("$> ");

		// take user input
		fgets(command, (MAXIN - 1), stdin);
		
		// remove stupid newline crap and prepare for no arg detection
		in_len = strlen(command);
		command[in_len - 1] = '\0';
		command[in_len] = '\0';
		
		// process user input
		in_argv = popFront(command);

		// act on user input
		//man
		if (!strcmp(command, "man")) {

			continue;
		}

		// calc
		if (!strcmp(command, "calc")) { 

			calc(in_argv);
			continue;

		}

		// time
		if (!strcmp(command, "time")) {
			
			myTime();
			continue;
		
		}

		// path
		if (!strcmp(command, "path")) {
			
			path();
			continue;

		}

		// sys
		if (!strcmp(command, "sys")) {
			
			sys();
			continue;

		}

		// put
		if (!strcmp(command, "put")) {
			
			put(in_argv);
			continue;

		}

		// get
		if (!strcmp(command, "get")){

			get(in_argv);
			continue;

		}

		// quit
		if (!strcmp(command, "quit"))
			return 0;

		// code for unknown command
		printf("unkown command found\n");

	}

	// return program error
	return 1;

}