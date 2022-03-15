// own includes
#include "client_includes.h"

// main program loop
int main(int argc, char* argv[]) {
	
	// initialise variables
	char * input_buffer;
	int status = 1;
	pthread_t communication_thread;
	void * res;

	// turn off printf buffering
	setbuf(stdout, NULL);

	// introduce program
	printf("Welcome to S5132483's client program for 2803ICT Assignment 3\n");

	// initialise mutex
	if (pthread_mutex_init(&send_request_control, NULL) != 0) {

		printf("ERROR: mutex init failed\n");
		return 1;
	
	}

	// allocate buffer
	input_buffer = (char*)calloc(LENVAL, sizeof(char));

	// get shared memory pointer
	sm_ptr = getSharedMemoryBind();

	if (sm_ptr == (struct Shared_Memory *) -1) {
		
		// present and return error
		printf("ERROR: shmat error, memory failed to bind\n");
		pthread_exit(NULL);
	
	}

	// begin thread which loops through looking to accept input from the server
	pthread_create(&communication_thread, NULL, communicationReceiver, (void *) NULL);

	// begin user interaction loop
	while (status == 1) {

		// initialise input carrier
		uint32_t * input_number = malloc(sizeof(uint32_t));

		// refresh input buffer
		memset(input_buffer, '\0', LENVAL);

		// get input
		fgets(input_buffer, LENVAL, stdin);
		
		// if the input exceeded the limit of the fgets then clear stdin
		if (input_buffer[strlen(input_buffer) - 1] != '\n')
			flushInBuffer();

		// check if the quit command has been given
		if (input_buffer[0] == 'q') {

			// send quit
			signalQuit();

			// change status and break
			status = 0;
			break;

		}
		sleep(0);
		// if only the enter button was pressed then restart the loop
		if (input_buffer[0] == '\n')
			continue;

		// check if the input is valid
		if (inValid(input_buffer) == 1) {

			*input_number = getInput(input_buffer);
			
			// initialise thread id
			pthread_t request_thread;

			// if trying to use the test mode
			if (*input_number == 0) {

				int usable = 1;

				// check to see if test mode can be used right now
				for (int x=0; x < SLOTS; x++)
					if (sm_ptr->status.slot_status[x] != (char) OPEN) {

						// warn the user
						printf("WARN: can not enter test mode as there are currently requests running\n");

						// signal to restart the loop
						usable = 0;
						break;

					}

				// check to see if test mode is already active
				if (sm_ptr->progress.slot_progress[0] == (char) TEST) {

					// warn the user
					printf("WARN: can not enter test mode as there are currently requests running\n");

					// signal to restart the loop
					usable = 0;

				}

				// restart the loop
				if (usable == 0) {

					free(input_number);
					continue;

				}

			} else
				// check to see if test mode is already active
				if (sm_ptr->progress.slot_progress[0] == (char) TEST) {

					// warn the user
					printf("WARN: can not send request right now as the server is currently in test mode\n");

					// restart the loop
					free(input_number);
					continue;

				}

			// begin a new thread which will wait to make a request to the server when appropriate
			pthread_create(&request_thread, NULL, requestSender, (void *) input_number);

		}

	}

	// collect communication thread
	pthread_join(communication_thread, &res);

	// destory mutex
	pthread_mutex_destroy(&send_request_control);

	// remove shared memory connection
	shmdt((void *) sm_ptr);

	// return program complete
	return 0;

}