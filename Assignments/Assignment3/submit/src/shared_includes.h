// pragma once
#pragma once

// defines
/*!
    GENSIZE = generic size : the default size allocated to a string
*/ 
#define GENSIZE 1024
/*!
    MINVAL = minimum value : the minimum size allowed for input
*/ 
#define MINVAL 0
/*!
    MAXVAL = maximum value : the maximum size allowed for input
*/ 
#define MAXVAL 4294967295
/*!
    LENVAL = value length : the maximum legth of input
*/ 
#define LENVAL 11
/*!
    SLOWDOWN = slow down ammount : a number used by strategic sleep commands throughout the program to induce a longer run time
*/ 
#define SLOWDOWN 0.7
/*!
    SLOTS = slots : the maximum number of slots the server has for taking requests
*/ 
#define SLOTS 10
/*!
    OPEN = slot is open : a flag signalling that a slot is ready to recieve a request
*/
#define OPEN 20
/*!
    REQUEST = a request in slot : a flag signalling that a slot contains a new request for the server 
*/ 
#define READY 21
/*!
    FILLED = slot filled : a flag signalling that a slot is currently filled with a value to be taken
*/
#define  REQUEST 22
/*!
    READY = slot ready : a flag signalling that a slot is ready to start processing the request on the clients signal server or that the server has finished a request
*/
#define FILLED 23
/*!
    taken = slot value taken : a flag signalling that the value that was in this slot has been taken
*/
#define TAKEN 24
/*!
    QUIT = the quit flag : this states that the client has quit and the server should end
*/
#define QUIT 25
/*!
    TEST = test mode : a flag signalling that the server is in test mode
*/
#define TEST 26

// includes
// general
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
// shared memory
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
// threads
#include <pthread.h>
#include <unistd.h>

// structs

// shared memory structure

/*!
    Number : is a shared variable struct that will
	be used to send and receive numbers between the
	server and the client.
*/ 
struct Number {

	uint32_t main_slot;
	uint32_t slot [SLOTS];

};
/*!
    Number_Handshake : is a shared variable struct
	that will be used for signalling states between
	the client and the server.
*/ 
struct Number_Handshake {

	char main_slot_status;
	char slot_status[SLOTS];

};

/*!
    Progress : is a shared variable struct that will
	be used for relaying the progress of each slot in
	its calculations.
*/ 
struct Progress {

	char slot_progress[SLOTS];

};

/*!
    Shared_Memory : is a shared variable struct that
	will be used as a container to hold a single instance
	of all the other shared memory structs.
*/ 
struct Shared_Memory {

	struct Number number;
	struct Number_Handshake status;
	struct Progress progress;

};

// shared memory
extern struct Shared_Memory  * sm_ptr;

// shared functions

/*!
    
	<summary>
        when called this function will return the time since it the given time
    </summary>

	<param name="start">a clock_t containing the start time to subtract from the current time</param>

    <returns>a double containing the time since the time given</returns>

*/
double timeElapsed (clock_t start);

/*!
    
	<summary>
        this function binds a variable to the shared memory
    </summary>

    <returns>a struct pointer of type Shared_Memory that bound to the shared memory</returns>

	useage should be followed by:
	// check the bind
	if (sm_ptr == (struct Shared_Memory *) -1) {
		
		// present and return error
		printf("ERROR: shmat error, memory failed to bind\n");
		return 1;
	
	}
	and:
	shmdt((void *) sm_ptr);
	when done

*/
struct Shared_Memory * getSharedMemoryBind();

/*!
    
	<summary>
        detects whether or not the quit signal has been sent
    </summary>

    <returns>an int either 1 or zero indicating whether or not the quit flag was detected</returns>

*/
int detectQuit ();