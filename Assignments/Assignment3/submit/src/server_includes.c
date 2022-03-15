// include self
#include "server_includes.h"

// mutexs
pthread_mutex_t slot_reply_control[SLOTS] = {
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER
} ;

pthread_mutex_t test_control = PTHREAD_MUTEX_INITIALIZER;

// functions
/*!
    
	<summary>
        this thread function handles the handling of a request to the server starting the various solvers
    </summary>

	<param name="arg">the arguments variable for the thread function, in this case it will pass an instance of request handler</param>

    <returns>void *</returns>

*/
void *requestHandler (void * arg) {

	// type cast arg
	struct request_information * ptr = (struct request_information *) arg;

	// initialise variables
	pthread_t solver_threads[SOLVERS];
	int * progress = (int *)malloc(SOLVERS*sizeof(int));

	// initialise progress
	for (int x=0; x < SOLVERS; x++)
		progress[x] = 0;

	// put slot number in the main slot
	sm_ptr->number.main_slot = ptr->slot;
	// set main slot status
	sm_ptr->status.main_slot_status = (char) READY;

	// wait until slot has been read
	while (sm_ptr->status.main_slot_status != (char) TAKEN) {}

	// inform server of start
	printf("slot %i has started computation\n", ptr->slot);

	// revert the main slot to open
	sm_ptr->status.main_slot_status = (char) OPEN;
	// change slot status to taken
	sm_ptr->status.slot_status[ptr->slot] = (char) TAKEN;

	// loop through creating threads
	for (int x=0; x < SOLVERS; x++) {

		// initialise request information holder
		struct solver_information * solver_package = malloc(sizeof(struct solver_information));

		// get solver information
		// set the progress holder
		solver_package->progress = progress;
		// set the slot number
		solver_package->slot = ptr->slot;
		// set solver number
		solver_package->solver = x;
		// set input number
		if (x != 0)
			solver_package->my_input = rightRotate(ptr->my_input, x);
		else
			solver_package->my_input = ptr->my_input;

		// send off solver thread
		pthread_create(&solver_threads[x], NULL, solver, (void *) solver_package);

	}

	// initialise progress holders
	int incomplete = 1;
	uint32_t percentage;
	double total;
	
	// loop through reporting progress
	while (incomplete == 1) {

		// reset variables
		incomplete = 0;
		percentage = 0;

		// loop through the solver threads
		for (int x=0; x < SOLVERS; x++) {


			if (progress[x] > 100)
				// add to total
				percentage = percentage + 100; 
			else
				// add to total
				percentage = percentage + progress[x]; 

			// check if given thread is incomplete
			if (progress[x] < 100)
				incomplete = 1;

		}

		// calculate average
		total = (double) (percentage / SOLVERS);
		percentage = (int) total;
	
		// sometimes dude numbers seem to come through here, not full certain why
		if (percentage >= 0 && 100 >= percentage)
			sm_ptr->progress.slot_progress[ptr->slot] = (char) percentage;

	}

	// loop through collecting threads
	for (int x=0; x < SOLVERS; x++)
		pthread_join(solver_threads[x], NULL);


	// set the slot status to request signifying it's completed the request
	sm_ptr->status.slot_status[ptr->slot] = (char) REQUEST;

	printf("slot %i computation complete\n", ptr->slot);
	sleep(1);

	// free malloc'd memory
	free(progress);
	free(ptr);

	// terminate thread
	pthread_exit(NULL);

}

/*!
    
	<summary>
        this thread function handles the indivdual solving of a given number derived from the input number of the requets
    </summary>

	<param name="arg">the arguments variable for the thread function, in this case it will pass the number to solve</param>

    <returns>void *</returns>

*/
void *solver (void * arg) {

	// type cast arg
	struct solver_information * ptr = (struct solver_information *) arg;

	// begin trial division
	uint32_t N = ptr->my_input, F = 2, marker = N;

	while (N > 1) {
		sleep(SLOWDOWN);

		if ((N % F) == 0) {

			// mutex lock protected section to send slot result back to the client
			pthread_mutex_lock(&slot_reply_control[ptr->slot]);
		

				// put the value into shared memory
				sm_ptr->number.slot[ptr->slot] = F;

				// change status flag
				sm_ptr->status.slot_status[ptr->slot] = (char) FILLED;

				// wait for user to take value
				while (sm_ptr->status.slot_status[ptr->slot] != (char) TAKEN) {}

			// unlock the mutex
			pthread_mutex_unlock(&slot_reply_control[ptr->slot]);

			N = (uint32_t) (N / F);

		} else
			F++;

		// record progress
		ptr->progress[ptr->solver] = (int) ((double) (((double) F / (double) marker) * (double) 100));
		
		// fix annoying overflow issue if it comes up again
		if (ptr->progress[ptr->solver] > 100)
			ptr->progress[ptr->solver] = 100;

	}

	// in case the calculations were off set progress to 100
	ptr->progress[ptr->solver] = 100;

	// free malloc'd memory
	free(ptr);

	// terminate thread
	pthread_exit(NULL);

}

// Function to check if a number is
// a prime number or not
void TrialDivision(uint32_t N){

	

}

/*!
    
	<summary>
        this fuction rotates a given unsigned long by a given number
    </summary>

	<param name="number">the number which will be rotated to the right</param>
	<param name="count">the number of times which the number should be rotated to right for</param>

    <returns>32 bit interger containing the time since the function was last called</returns>

*/
uint32_t rightRotate(uint32_t number, int count) {

    return (number >> count)|(number << (32 - count));

}

/*!
    
	<summary>
        this fuction runs the test mode for the server
    </summary>

    <returns>void</returns>

*/
void runTestMode() {

	// initialise thread id
	pthread_t request_thread[3];

	// open main slot
	sm_ptr->status.main_slot_status = (char) OPEN;

	// start the three test sets
	for (int x=0; x < 3; x++) {

		// initialise thread payload
		struct test_holder * payload = malloc(sizeof(struct test_holder));

		// wait for open
		while (sm_ptr->status.main_slot_status != (char) OPEN) {}

		// set slot
		payload->slot = -1;

		// find an empty slot
		while (payload->slot == -1) 
			for (int x=0; x < SLOTS; x++)
				if (sm_ptr->status.slot_status[x] == (char) OPEN) {

					payload->slot = x;
					sm_ptr->status.slot_status[x] = (char) READY;
					break;

				}

		// change main slot status
		sm_ptr->status.main_slot_status = (char) TAKEN;

		printf("new test going to slot %i\n", payload->slot);

		// begin a new thread to process the test handler
		pthread_create(&request_thread[x], NULL, testHandler, (void *) payload);

	}

	// join back to the handler threads and reset slots
	for (int x=0; x < 3; x++) {

		pthread_join(request_thread[x], NULL);

	}

	// return the server to normal operations
	sm_ptr->progress.slot_progress[0] = (char) 0;

	sleep(1);

	for (int x=0; x < 3; x++) {

		// wait for client
		while (sm_ptr->status.slot_status[x] != (char) TAKEN) {}

		// set to requets to signify end
		sm_ptr->status.slot_status[x] = (char) REQUEST;

	}

}

/*!
    
	<summary>
        this thread function handles the handling of the test threads
    </summary>

	<param name="arg">the arguments variable for the thread function, in this case it will pass an instance of test_holder</param>

    <returns>void *</returns>

*/
void *testHandler (void * arg) {

	// type cast arg
	struct test_holder * ptr = (struct test_holder *) arg;

	// initialise variables
	pthread_t test_threads[10];

	// mutex lock protected section to send slot result back to the client
	pthread_mutex_lock(&test_control);

		// put slot number in the main slot
		sm_ptr->number.main_slot = ptr->slot;
		// set main slot status
		sm_ptr->status.main_slot_status = (char) READY;

		// wait until slot has been read
		while (sm_ptr->status.main_slot_status != (char) TAKEN) {}

		// revert the main slot to open
		sm_ptr->status.main_slot_status = (char) OPEN;

		// change slot status to taken
		sm_ptr->status.slot_status[ptr->slot] = (char) TAKEN;

	// unlock the mutex
	pthread_mutex_unlock(&test_control);

	// inform server of start
	printf("slot %i has started test\n", ptr->slot);

	// loop through creating threads
	for (int x=0; x < 10; x++) {
		
		// initialise test information holder
		struct test_holder * payload = malloc(sizeof(struct test_holder));

		// get test information
		// set the number
		payload->my_number = (int) ((x + 1) * 10);
		// set the slot number
		payload->slot = ptr->slot;

		// send off test thread
		pthread_create(&test_threads[x], NULL, tester, (void *) payload);

	}

	// loop through collecting threads
	for (int x=0; x < 3; x++)
		pthread_join(test_threads[x], NULL);

	printf("slot %i computation complete\n", ptr->slot);

	// wait until slot has been read
	while (sm_ptr->status.slot_status[ptr->slot] != (char) TAKEN) {}

	// free malloc'd memory
	free(ptr);

	// terminate thread
	return NULL;

}

/*!
    
	<summary>
        this thread function handles the indivdual solving of a given number derived from the input number of the requets
    </summary>

	<param name="arg">the arguments variable for the thread function, in this case it will pass an instance of test_holder</param>

    <returns>void *</returns>

*/
void *tester (void * arg) {

	// type cast arg
	struct test_holder * ptr = (struct test_holder *) arg;

	// initialise timer start and random number generator
	clock_t start;
	double wait_time;

	// begin sending back values
	for (int x=ptr->my_number; x < (ptr->my_number + 10); x++) {

		// get start time
		start = clock();
		
		// get random wait time
		wait_time = (double) ((double) (rand() % (100 + 1 - 10) + 10) / (double) 1000);

		// begin sending loop
		while(1) {

			// if enough time has passed send the number
			if (timeElapsed(start) >= wait_time) {

				// mutex lock protected section to send slot result back to the client
				pthread_mutex_lock(&slot_reply_control[ptr->slot]);

					// put the value into shared memory
					sm_ptr->number.slot[ptr->slot] = x;

					// change status flag
					sm_ptr->status.slot_status[ptr->slot] = (char) FILLED;

					// wait for user to take value
					while (sm_ptr->status.slot_status[ptr->slot] != (char) TAKEN) {}

				// unlock the mutex
				pthread_mutex_unlock(&slot_reply_control[ptr->slot]);

				// break the while loop
				break;

			}

		}

	}

	// free malloc'd memory
	free(ptr);

	// terminate thread
	return NULL;

}