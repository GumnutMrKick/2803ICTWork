#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// #include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "includes.h"

// include self
#include "functions.h"

///////// main commands /////////

// calculates the given 'expr' exspression
void calc (char* in_argv) {

	// check for args
	if (checkArgs(in_argv)) {

		printf("no arguments recieved\n");
		return;

	}

	// initialise some variables
	int answer, length;

	// find the string length
	length = strlen(in_argv);

	// calculate and announace the answer
	printf("The answer is: %d\n", ulate(in_argv, 0, length));

}

// displays the current location appropriate time
void myTime () {

	// variable definition
	time_t curr_time;
	struct tm * format;

	// get and translate time
	time(&curr_time);
	format = localtime(&curr_time);

	// display time
	printf("the current local date and time is: %s", asctime(format));

}

// prints out the current working directory
void path () {

	// variable definition
	char dir[MAXIN];

	if (getcwd(dir, sizeof(dir)) == NULL) {

		printf("there was an issue getting the directory\n");
		return;

	}

	printf("the current working directory is: %s\n", dir);

}

// prints the name, version of the OS and CPU type
void sys () {



}

// puts files of 'filenames' in the given 'dirname' directory
void put (char* in_argv) {

	// check for args
	if (checkArgs(in_argv)) {

		printf("no arguments recieved\n");
		return;

	}

	// give disclaimer
	printf("please note that this will not work as desired if the filenames have spaces\n");

	// declare variables
	char * files = popFront(in_argv), * flag_addr, directory[MAXIN], * command, line[MAXIN];
	FILE * del_dir;
	int line_num = 0;

	printf("%s", files);

	// check for the flag
	flag_addr = strstr(files, " -f");

	// if flag is present then remove it from files args
	if (flag_addr != NULL) {
		flag_addr[0] = '\0';
		printf("flag found");
	}

	printf("%s\n", files);

	printf("hello");

	// generate directory path
	directory[0] = '\0';
	strcat(directory, "./");
	printf("%s\n", directory);

	strcat(directory, in_argv);
	printf("%s\n", directory);
	strcat(directory, "/");
	printf("%s\n", directory);

	printf("i GOT HERE\n");

	printf("\nthe answer%s\n", checkDirExistance(directory));

	if (checkDirExistance(directory))
		printf("dir alrady exists");

	// 	// delete the directory if flag was present
	// 	if (flag_addr) {

	// 		printf("flag exists");

	// 		// generate the command
	// 		command = strcat(strcat("rm -r \"", directory), "\"");

	// 		// run the command
	// 		del_dir = popen(command, "r");

	// 		// get the result
	// 		if (del_dir != NULL) {



	// 			// print the lines
	// 			while (fgets(line, sizeof(line), del_dir)) {

	// 				// print the line
	// 				printf("line %i : %s\n", line_num, line);

	// 				// pause if the current line number is divisable by 40
	// 				if((line_num % 40) == 0)
	// 					getchar();

	// 				// increment the line number
	// 				line_num++;

	// 			}


	// 		}

	// 	// if it wasn't present then error
	// 	} else {

	// 		printf("an error has occured, that directory already exists,\n add the flag -f to the end if you would like to overwrite\n");
	// 		return;

	// 	}
	// }

	// create the directory


	// copy the files to the directory
	// copyFiles(files, directory);

}

// prints the contents of the file at 'filename'
void get (char * in_file_name) {

	// check for args
	if (checkArgs(in_file_name)) {

		printf("no arguments recieved\n");
		return;

	}

	// variable definition
	FILE * file;
	char * file_name, line[MAXIN];
	int line_num = 1;

	// clean the file name in the arguments
	popFront(in_file_name);

	// open the file
	file = fopen(in_file_name, "r");

	if (file == NULL) {

		printf("there was an issue opening the file\n");
		return;

	}

	// print the lines
	while (fgets(line, sizeof(line), file)) {

		// print the line
		printf("line %i : %s\n", line_num, line);

		// pause if the current line number is divisable by 40
		if((line_num % 40) == 0)
			getchar();

		// increment the line number
		line_num++;

	}

}

///////// /main commands /////////
///////// supporting commands /////////

// pops off the front of the string to the first instance of a space
// returning the new array and terminating the old with \0 making it the popped
char* popFront (char * str) {

	for (int x=0; x < (MAXIN - 1); x++)
		if ((str[x] == '\0') || (str[x] == ' ')) {

			str[x] = '\0';
			return (str + x + 1);

		}

}

// detects if the supplied string had been marked as empty by the input in main
bool checkArgs (char *in_argv) {

	return((in_argv[0] == '\0'));

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

// checks for the existance of a directory
bool checkDirExistance (const char * dir_path) {


	printf("fuck");
	// prepare to make check
    struct stat info;

	// make check
    int statRC = stat( path, &info );


	printf("%i\n", statRC);
	printf("%s\n", ( info.st_mode & S_IFDIR ));

    if( statRC != 0 )
    {
        return -1;
    }


    return ( info.st_mode & S_IFDIR ) ? 1 : 0;


}

// once given a list of files and a destination, this function will
// recursivly attempt to copy each one to the desired location
int copyFiles (char * files, char * dest) {

	// test for end
	if (checkArgs(files))
		return 0;

	// variable definition
	char * new_files = popFront(files);

	// declare attempt start
	printf("copying %s : ... ", files);

	// attempt to copy file


	// try to copy next file
	return (1 + copyFiles(new_files, dest));

}

///////// /supporting commands /////////