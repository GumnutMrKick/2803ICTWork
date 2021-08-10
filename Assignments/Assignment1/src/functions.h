#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "includes.h"

///////// main commands /////////

// calculates the given 'expr' exspression
void calc (char* argv);

// displays the current location appropriate time
void time ();

// prints out the current working directory
void path ();

// prints the name, version of the OS and CPU type
void sys ();

// puts files of 'filenames' in the given 'dirname' directory
void put (char* argv);

// prints the contents of the file at 'filename'
void get (char* argv);

///////// /main commands /////////
///////// supporting commands /////////

// a function which prints describes the various commands avaliable to the user
void man ();

// appropriatly processes the users input
char* processInput (char* in_buffer);

// recursively extracts and runs mathimatical operations on a strings numbers and symbols
int ulate (char* str, int start, int end);


///////// /supporting commands /////////

// a test function
int foo (char* str);