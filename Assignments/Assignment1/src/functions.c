#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "includes.h"

// include self
#include "functions.h"

///////// main commands /////////

// calculates the given 'expr' exspression
void calc (char* argv) {

	// initialise some variables
	int answer, length;

	// find the string length
	length = strlen(argv);

	// calculate and announace the answer
	printf("The answer is: %d", ulate(argv, 0, length));

}

// displays the current location appropriate time
void time () {



}

// prints out the current working directory
void path () {



}

// prints the name, version of the OS and CPU type
void sys () {



}

// puts files of 'filenames' in the given 'dirname' directory
void put (char* argv) {



}

// prints the contents of the file at 'filename'
void get (char* argv) {



}

///////// /main commands /////////
///////// supporting commands /////////

// a function which prints describes the various commands avaliable to the user
void man () {



}

// appropriatly processes the users input
char* processInput (char* in_buffer) {

	for (int x=0; x < (MAXIN - 1); x++)
		if ((in_buffer[x] == '\0') || (in_buffer[x] == ' ')) {

			in_buffer[x] = '\0';
			return (in_buffer + x + 1);

		}

}

// recursively extracts and runs mathimatical operations on a strings numbers and symbols
int ulate (char* str, int start, int end) {

	// look for a sign
	for (int x=start; x < end; x++) {

		// if the character is a +
		if (str[x] == '+') {
			
			str[x] == '\0';
			return (ulate(str, start, x-1) + ulate(str, x+1, end));

		}

		// if the character is a -
		if (str[x] == '-') {

			str[x] == '\0';
			return (ulate(str, start, x-1) - ulate(str, x+1, end));

		}

	}

	// if no sign has been found then return the string as a number
	return atoi(str + start);	

}

///////// /supporting commands /////////

// a test function
int foo (char* str) {

	printf("hello %s", str);
	return 0;

}