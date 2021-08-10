#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "includes.h"
#include "functions.h"

// main program loop
int main(int argc, char* argv[]) {
	
	// check to see if man has been run
	printf("%d", argc);

	// running flag
	bool running = true;
	char in_buffer[MAXIN], *in_argv;

	// introduce program
	printf("Welcome to S5132483's shell program for 2803ICT Assignment 1\n");
	printf("PLEASE NOTE: all input has an arbitrary limit of 1000 characters ");

	// main shell loop
	while (running) {

		printf("\n\n$> ");

		// take user input
		scanf("%1000s", in_buffer);


		printf("%s", in_buffer);

		// process user input
		in_argv = processInput(in_buffer);

		// printf("b", (in_buffer == "calc"));



		// act on user input
		//man
		if (in_buffer == "man") {

		}

		// calc
		if (in_buffer == "calc") { 
		
			calc(in_argv);
			continue;

		}

		// time
		if (in_buffer == "time") {
			
			time();
			continue;
		
		}

		// path
		if (in_buffer == "path") {
			
			path();
			continue;

		}

		// sys
		if (in_buffer == "sys") {
			
			sys();
			continue;

		}

		// put
		if (in_buffer == "put") {
			
			put(in_argv);
			continue;

		}

		// get
		if (in_buffer == "get"){

			get(in_argv);
			continue;

		}

		// quit
		if (in_buffer == "quit")
			return 0;

		// code for unknown command
		printf("unkown command found, launch program with 'man' argument or run the man command to see command documentation.");

	}

	// return program error
	return 1;

}