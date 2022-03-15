// pragma once
#pragma once

// includes
#include "shared_includes.h"

// mutex
// send request control : is a mutex responsible for stopping the race
// condition when sending requests to the server 
extern pthread_mutex_t send_request_control;

// functions
/*!
    
	<summary>
        this thread function handles the recieving of any messages from the server
    </summary>

	<param name="arg">the arguments variable for the thread function</param>

    <returns>void *</returns>

*/
void *communicationReceiver (void * arg);

/*!
    
	<summary>
        this thread function handles making a request to the server when appropriate
    </summary>

	<param name="arg">the arguments variable for the thread function, in this case it will pass the number to be requested to the server</param>

    <returns>void *</returns>

*/
void *requestSender (void * arg);

/*!
    
	<summary>
        this function flushes the input buffer
    </summary>

    <returns>void</returns>

*/
void flushInBuffer ();

/*!
    
	<summary>
        returns whether or not the given input is valid
    </summary>

	<param name="input">an int containing the input to be checked</param>

    <returns>int as either 1 or 0</returns>

*/
int inValid (char input[LENVAL]);

/*!
    
	<summary>
        processes user input and returns the number
    </summary>

	<param name="input">an int containing the input to be checked</param>

    <returns>unsigned long containing the output as a number</returns>

*/
uint32_t getInput (char * input);

/*!
    
	<summary>
        sets all of the status variables to quit, with the hope that surely the server will see one
    </summary>

    <returns>void</returns>

*/
void signalQuit ();