// include self
#include "shared_includes.h"

// initialise shared memory holder
struct Shared_Memory  * sm_ptr = (struct Shared_Memory *) -1;

// shared functions

/*!
    
	<summary>
        when called this function will return the time since it the given time
    </summary>

	<param name="start">a clock_t containing the start time to subtract from the current time</param>

    <returns>double containing the time since the time given</returns>

*/
double timeElapsed (clock_t start) {

	clock_t end = clock();

	return ((double) (end - start) / CLOCKS_PER_SEC);

}

/*!
    
	<summary>
        this fuction gets a binded variable to the shared memory
    </summary>

    <returns>a struct pointer of type Shared_Memory that bound to the shared memory</returns>

	useage should be followed by:
	// check the bind
	if ((int) memory_ptr == -1) {
		
		// present and return error
		printf("ERROR: shmat error, memory failed to bind\n");
		return 1;
	
	}
	and:
	shmdt((void *) memory_ptr);
	when done

*/
struct Shared_Memory * getSharedMemoryBind() {

	key_t memory_key;
	int memory_id;

	// generate key
	memory_key = ftok(".", 'x');
	// share the memory	
	memory_id = shmget(memory_key, sizeof(struct Shared_Memory), 0666);

	// check the share
	if (memory_id < 0) {
	
		// present and return error
		printf("ERROR: shmget error, memory failed to share\n");
		return (struct Shared_Memory *) -1;

	}

	// bind the memory
	return (struct Shared_Memory *) shmat(memory_id, NULL, 0);

}

/*!
    
	<summary>
        detects whether or not the quit order has been sent
    </summary>

    <returns>int either 1 or zero indicating whether or not the quit status was detected</returns>

*/
int detectQuit () {

	// check main slot
	if (sm_ptr->status.main_slot_status == (char) QUIT) {
		
		return 1;

	}

	// check other slots
	for (int x=0; x < SLOTS; x++)
		if (sm_ptr->status.slot_status[x] == (char) QUIT) {

			return 1;

		}

	// return result
	return 0;

}