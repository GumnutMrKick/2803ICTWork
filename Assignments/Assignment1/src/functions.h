#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "includes.h"

///////// main commands /////////

// calculates the given 'expr' exspression
void calc (char* in_argv);

// displays the current location appropriate time
void myTime ();

// prints out the current working directory
void path ();

// prints the name, version of the OS and CPU type
void sys ();

// puts files of 'filenames' in the given 'dirname' directory
void put (char* in_argv);

// prints the contents of the file at 'filename'
void get (char * in_file_name);

///////// /main commands /////////
///////// supporting commands /////////

// pops off the front of the string to the first instance of a given character
// returning what was popped
char* popFront (char * str);

// detects if the supplied string had been marked as empty by the input in main
bool checkArgs (char *in_argv);

// a function which prints describes the various commands avaliable to the user
void man ();

// recursively extracts and runs mathimatical operations on a strings numbers and symbols
int ulate (char* str, int start, int end);

// checks for the existance of a directory
int checkDirExistance (char * dir_path);

// once given a list of files and a destination, this function will
// recursivly attempt to copy each one to the desired location
int copyFiles (char * files, char * dest);

///////// /supporting commands /////////