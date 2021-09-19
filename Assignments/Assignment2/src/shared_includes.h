/*!
    MAXIN = a global constant contains as an integer the maximum number of characters accepted in all input applications
*/ 
#define MAXIN 1001

/*!
    MAXIN = a global constant contains as an integer describing the maxium ammount of times either program should reattempt a failed
	network task with the hope that it will succeed next time
*/ 
#define MAXATTEMPTS 5

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// shared functions

/*!
    
	<summary>
        this function prints the current time to console
    </summary>

    <returns>void<returns>

*/
void myTime ();

/*!
    
	<summary>
        this function replaces the first space it finds with a \0 and return a char*
		pointer to the character after it
    </summary>

	<param name="str">the string which this function concerns</param>

    <returns>a char pointer to the string after the first<returns>

*/
char* popFront (char * str);

/*!
    
	<summary>
        first of all, frankly this feels like bad design
		but this function reverses the effect of the popFront function. upon giving it
		the orginal string it will remove the \0 which seperated the command from the args
    </summary>

	<param name="str">the string which this function concerns</param>

    <returns>void<returns>

*/
void unPopFront (char * str);

/*!
    
	<summary>
        this function cleans up after fget's crap
    </summary>

	<param name="str">the string which this function concerns</param>

    <returns>void<returns>

*/
void fgetsClean (char * str);