// include self
#include "client_includes.h"

// initialise mutex holder
pthread_mutex_t send_request_control = PTHREAD_MUTEX_INITIALIZER;

// functions
/*!
    
	<summary>
        this thread function handles the recieving of any messages from the server
    </summary>

	<param name="arg">the arguments variable for the thread function</param>

    <returns>void *</returns>

*/
void *communicationReceiver (void * arg) {

	// initialise time handles and progress handlers
	clock_t times[SLOTS], timeout;
	int fill, last_print_progress = 0;
	timeout = clock();

	// begin loop
	while (detectQuit() == 0) {

		// check for starting
		if (sm_ptr->status.main_slot_status == (char) READY) {

			// start timer
			times[sm_ptr->number.main_slot] = clock();
			
			// reset timeout and flag
			timeout = clock();
			last_print_progress = 0;
			
			// inform the user
			printf("request starting in slot %i\n", sm_ptr->number.main_slot);
			// change flag
			sm_ptr->status.main_slot_status = (char) TAKEN;

		}

		// check slots
		for (int x=0; x < SLOTS; x++) {

			// check for output
			if (sm_ptr->status.slot_status[x] == (char) FILLED) {

				// print output
				printf("slot %i has returned %lu\n", x, sm_ptr->number.slot[x]);

				// change flag
				sm_ptr->status.slot_status[x] = (char) TAKEN;
				
				// reset timeout and flag
				timeout = clock();
				last_print_progress = 0;

			// check for completion
			} else if (sm_ptr->status.slot_status[x] == (char) REQUEST) {

				// print output
				printf("slot %i has completed taking %f seconds\n", x, timeElapsed(times[x]));

				// change flag
				sm_ptr->status.slot_status[x] = (char) OPEN;
				
				// reset timeout and flag
				timeout = clock();
				last_print_progress = 0;

			}

		}

		// check for running slots
		int running_slots = 0;
		for (int x=0; x < SLOTS; x++)
			if (sm_ptr->status.slot_status[x] != (char) OPEN) {
				
				running_slots = 1;
				break;

			}

		// check for pregress report timeout
		if ((running_slots == 1) && ((timeElapsed(timeout) > (double) 0.5) && (sm_ptr->progress.slot_progress[0] != (char) TEST))) {

			// return to the start of the line if the last line printed was a progress report
			if (last_print_progress = 1)
				printf("\r");

			// check slots
			for (int x=0; x < SLOTS; x++) {

				if (sm_ptr->status.slot_status[x] != (char) OPEN) {
					
					fill = (int) sm_ptr->progress.slot_progress[x];
					
					printf("SLOT %i %i %% ", (x + 1), fill);
					
					fill = (int) ((double) 20 *  (double) ((double) fill / (double) 100));

					for (int x=0; x < fill; x++)
						printf("▓");

					for (int x=0; x < (20 - fill); x++)
						printf("_");

					printf("|");

				}

			}

			printf("\n");

			// reset timeout and flag
			timeout = clock();
			last_print_progress = 1;
		
		}

	}

	// terminate thread
	pthread_exit(NULL);

}

/*!
    
	<summary>
        this thread function handles making a request to the server when appropriate
    </summary>

	<param name="arg">the arguments variable for the thread function, in this case it will pass the number to be requested to the server</param>

    <returns>void *</returns>

*/
void *requestSender (void * arg) {

	// get input number
	uint32_t * request_number = (uint32_t *) arg; 

	// check to see if the player should be warned
	int one_free = 0;

	for (int x=0; x < SLOTS; x++)
		if (sm_ptr->status.slot_status[x] == (char) OPEN) {

			one_free = 1;
			break;

		}

	if (one_free == 0)
		printf("please note, the server is currently busy and you may be in for a wait\n");

	// mutex lock protected section to make request
	pthread_mutex_lock(&send_request_control);

		// initialise counter
		one_free = 0;

		// loop until a slot is free
		while (one_free == 0) {

			// check for a free slot
			for (int x=0; x < SLOTS; x++)
				if ((sm_ptr->status.slot_status[x] == (char) OPEN) && (sm_ptr->status.main_slot_status == (char) OPEN)) {

					one_free = 1;
					break;

				}

		}

		// send the request
		// give number
		sm_ptr->number.main_slot = *request_number;
		// change status
		sm_ptr->status.main_slot_status = (char) REQUEST;

	// unlock the mutex
	pthread_mutex_unlock(&send_request_control);

	// free malloc'd memory
	free(request_number);

	// terminate thread
	pthread_exit(NULL);

}

/*!
    
	<summary>
        this function flushes the input buffer
    </summary>

    <returns>void</returns>

*/
void flushInBuffer () {

	int ch;
	while((ch = getchar()) != '\n' && ch != EOF);

}

/*!
    
	<summary>
        returns whether or not the given input is valid
    </summary>

	<param name="input">an int containing the input to be checked</param>

    <returns>int as either 1 or 0</returns>

*/
int inValid (char * input) {

	int ch, x = 0;

	// loop through the given string
	while (x < LENVAL) {

		// get newest character 
		ch = input[x];
		
		// if a new line or null character is found then break the loop
		// as the end has been found
		if (ch == '\0' || ch == '\n')
			break;

		// if the given character is not a number 
		if (!(48 <= ch && ch <= 57)) {

			// present and return error
			printf("ERROR: given input contains a non numeric character\n");
			return 0;

		}

		x++;

	}

	return 1;

}

/*!
    
	<summary>
        processes user input and returns the number
    </summary>

	<param name="input">an int containing the input to be checked</param>

    <returns>unsigned long containing the output as a number</returns>

*/
uint32_t getInput (char * input) {

	char * eptr;

	return (uint32_t) strtoul(input, &eptr, 10);

}

/*!
    
	<summary>
        sets all of the status variables to quit, with the hope that surely the server will see one
    </summary>

    <returns>void</returns>

*/
void signalQuit () {

	// initialise shared memory holder
	struct Shared_Memory  * sm_ptr;

	// get shared memory pointer
	sm_ptr = getSharedMemoryBind();

	// set main slot
	sm_ptr->status.main_slot_status = (char) QUIT;

	// set other slots
	for (int x=0; x < SLOTS; x++)
		sm_ptr->status.slot_status[x] = (char) QUIT;

}