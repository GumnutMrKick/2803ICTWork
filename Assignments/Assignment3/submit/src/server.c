// own includes
#include "shared_includes.h"
#include "server_includes.h"

// main program loop
int main(int argc, char* argv[]) {
	
	// turn off printf buffering
	setbuf(stdout, NULL);

	// initialise variables
	key_t memory_key;
	int memory_id;
	uint32_t input_number;
	struct Shared_Memory *memory_ptr;

	// introduce program
	printf("Welcome to S5132483's game_server program for 2803ICT Assignment 2\n");

	// initialise mutexs
	for (int x=0; x < SLOTS; x++)
		if (pthread_mutex_init(&slot_reply_control[x], NULL) != 0) {

	        printf("ERROR: mutex init %i failed\n", x);
    	    return 1;
    	
		}

	if (pthread_mutex_init(&test_control, NULL) != 0) {

		printf("ERROR: test mutex init failed\n");
		return 1;
	
	}

	// setup shared memory
	// generate key
	memory_key = ftok(".", 'x');
	// share the memory	
	memory_id = shmget(memory_key, sizeof(struct Shared_Memory), IPC_CREAT | 0666);
	// check the share
	if (memory_id < 0) {
	
		// present and return error
		printf("ERROR: shmget error, memory failed to share\n");
		return 1;

	}
	// bind the memory
	memory_ptr = (struct Shared_Memory *) shmat(memory_id, NULL, 0);
	// check the bind
	if (memory_ptr == (struct Shared_Memory *) -1) {
		
		// present and return error
		printf("ERROR: shmat error, memory failed to bind\n");
		return 1;
	
	}

	// get shared memory pointer
	sm_ptr = getSharedMemoryBind();

	if (sm_ptr == (struct Shared_Memory *) -1) {
		
		// present and return error
		printf("ERROR: shmat error, memory failed to bind\n");
		pthread_exit(NULL);
	
	}

	// initialise shared memory
	// number
	memory_ptr->number.main_slot = (uint32_t) 0;
	for (int x=0; x<SLOTS; x++)
		memory_ptr->number.slot[x] = (uint32_t) 0;
	// status
	memory_ptr->status.main_slot_status = (char) OPEN;
	for (int x=0; x<SLOTS; x++)
		memory_ptr->status.slot_status[x] = (char) OPEN;
	// progress
	for (int x=0; x<SLOTS; x++)
		memory_ptr->progress.slot_progress[x] = (char) 0;

	// begin loop
	while (detectQuit() == 0) {

		// check for new request
		if (memory_ptr->status.main_slot_status == (char) REQUEST) {

			// check for test mode
			if (memory_ptr->number.main_slot == 0) {

				runTestMode();

			} else {

				// initialise request information holder
				struct request_information  * request = malloc(sizeof(struct request_information));

				// get request number
				request->my_input = memory_ptr->number.main_slot;
				request->slot = -1;

				// find an empty slot
				while (request->slot == -1) 
					for (int x=0; x < SLOTS; x++)
						if (memory_ptr->status.slot_status[x] == (char) OPEN) {

							request->slot = x;
							memory_ptr->status.slot_status[x] = (char) READY;
							break;

						}

				// change main slot status
				memory_ptr->status.main_slot_status = (char) TAKEN;

				printf("new request going to slot %i, with input %lu\n", request->slot, request->my_input);

				// initialise thread id
				pthread_t request_thread;

				// begin a new thread to process the request
				pthread_create(&request_thread, NULL, requestHandler, (void *) request);
				
				// detach the thread
				pthread_detach(request_thread);

			}

		}

	}

	// detach memory
	shmdt((void *) memory_ptr);
	// clear memory
	shmctl(memory_id, IPC_RMID, NULL);

	// destory mutexs
	for (int x=0; x < SLOTS; x++)
		pthread_mutex_destroy(&slot_reply_control[x]);

	// remove shared memory connection
	shmdt((void *) sm_ptr);

	// return program complete
	return 0;

}