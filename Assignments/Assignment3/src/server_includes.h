// pragma once
#pragma once

// defines
/*!
    SLOVERS = solving threads count : the number of solving threads which must be started
*/ 
#define SOLVERS 32

// includes
#include "shared_includes.h"

// structs
/*!
    request_information : is a struct that will be given to the request handler
*/ 
struct request_information {

	int slot;
	uint32_t my_input;

};
/*!
    solver_progress : is a struct that will be given to each solver allowing them to report their progress
*/ 
struct solver_information {

	int * progress;
	int slot;
	int solver;
	uint32_t my_input;

};

/*!
    test_holder : is a struct that will be given to each of the test threads
*/ 
struct test_holder {

	int my_number;
	int slot;

};


// mutexs
/*!
    slot reply control : is a mutex responsible for stopping
	the race condition when sending answers back to the client 
*/ 
extern pthread_mutex_t slot_reply_control[SLOTS];
/*!
    test request control : is a mutex responsible for stopping the
	race condition when enacting dummy requests from the server
*/
extern pthread_mutex_t test_control;

// functions
/*!
    
	<summary>
        this thread function handles the handling of a request to the server starting the various solvers
    </summary>

	<param name="arg">the arguments variable for the thread function, in this case it will pass an instance of request handler</param>

    <returns>void *</returns>

*/
void *requestHandler (void * arg);

/*!
    
	<summary>
        this thread function handles the indivdual solving of a given number derived from the input number of the requets
    </summary>

	<param name="arg">the arguments variable for the thread function, in this case it will pass the number to solve</param>

    <returns>void *</returns>

*/
void *solver (void * arg);

/*!
    
	<summary>
        this fuction rotates a given unsigned long by a given number
    </summary>

    <returns>32 bit interger containing the time since the function was last called</returns>

*/
uint32_t rightRotate(uint32_t n, int b);

/*!
    
	<summary>
        this function runs the test mode for the server
    </summary>

    <returns>void</returns>

*/
void runTestMode();

/*!
    
	<summary>
        this thread function handles the handling of the test threads
    </summary>

	<param name="arg">the arguments variable for the thread function, in this case it will pass an instance of test_holder</param>

    <returns>void *</returns>

*/
void *testHandler (void * arg);

/*!
    
	<summary>
        this thread function handles the indivdual solving of a given number derived from the input number of the requets
    </summary>

	<param name="arg">the arguments variable for the thread function, in this case it will pass an instance of test_holder</param>

    <returns>void *</returns>

*/
void *tester (void * arg);