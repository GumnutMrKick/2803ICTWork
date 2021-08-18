#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <unistd.h>
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

	// variable definition
	FILE * os, * cpu;
	char line[MAXIN], os_command[MAXIN], cpu_command[MAXIN];
	int line_num = 1;

	// create the commands
	os_command[0] = '\0';
	strcat(os_command, "cat /etc/os-release | egrep 'PRETTY_NAME'");
	
	cpu_command[0] = '\0';
	strcat(cpu_command, "lscpu | egrep 'Model name'");

	// run the commands
	os = popen(os_command, "r");
	cpu = popen(cpu_command, "r");

	// check results
	while (fgets(line, sizeof(line), os))
		// print the line
		printf("%s", line);

	// check results
	while (fgets(line, sizeof(line), cpu))
		// print the line
		printf("%s", line);

	// close
	pclose (os);
	pclose (cpu);

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
	char * files = popFront(in_argv), * flag_addr, directory[MAXIN], command[MAXIN];
	FILE * del_dir, * make_dir;

	// check for the flag
	flag_addr = strstr(files, " -f");

	// generate directory path
	directory[0] = '\0';
	strcat(directory, "./");
	strcat(directory, in_argv);
	strcat(directory, "/");

	// check for the directories existance
	if (checkDirExistance(directory) == 1)
		// delete the directory if flag was present
		if (flag_addr) {
			
	 		// generate the command
	 		command[0] = '\0';
			strcat(command, "rm -r \"");
			strcat(command, directory);
			strcat(command, "\"");
			
	 		// run the command
	 		del_dir = popen(command, "r");

			// get the result
			if (del_dir != NULL)
				printf("%s was deleted as requested\n", directory);

			// close
			pclose (del_dir);

		// if it wasn't present then error
		} else {

			printf("an error has occured, that directory already exists,\n add the flag -f to the end if you would like to overwrite\n");
			return;

		}

	// if flag is present then remove it from files args
	if (flag_addr != NULL) {
	
		flag_addr[0] = '\0';
		flag_addr[1] = '\0';
		flag_addr[2] = '\0';

	}

	// create the directory

	// generate the command
	command[0] = '\0';
	strcat(command, "mkdir \"");
	strcat(command, in_argv);
	strcat(command, "\"");

	// run the command
	make_dir = popen(command, "r");

	// close
	pclose (make_dir);

	// copy the files to the directory
	copyFiles(files, directory);

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
int checkDirExistance (char * dir_path) {

	// prepare to make check
    struct stat info;

	// make check
    int statRC = stat(dir_path, &info );

	// was there an error
    if (statRC != 0 )
		return -1;

	// return answer
    return ( info.st_mode & S_IFDIR ) ? 1 : 0;

}

// once given a list of files and a destination, this function will
// recursivly attempt to copy each one to the desired location
int copyFiles (char * files, char * dest) {

	// test for end
	if (checkArgs(files))
		return 0;

	// variable definition
	FILE * copy;
	char * new_files = popFront(files), command[MAXIN];

	// attempt to copy file
	// create the command
	command[0] = '\0';
	strcat(command, "cp \"");
	strcat(command, files);
	strcat(command, "\" \"");
	strcat(command, dest);
	strcat(command, "\"");

	// run the command - cp will print to console if there is an issue
	copy = popen(command, "r");

	// close
	pclose (copy);

	// try to copy next file
	return (1 + copyFiles(new_files, dest));

}

///////// /supporting commands /////////