/*!
    BACK_LOG = a global constant contains as an integer of the maximum number of clients that can attempt to connect at the same time
*/ 
#define BACK_LOG 10

/*!
    GAME_GOAL = a global constant contains as an integer of the goal number to be reached in the game
*/ 
#define GAME_GOAL 30

/*!
    TIMEOUT_LIMTI = the ammount of time the server will wait for a client answer after giving them the go command
*/ 
#define TIMEOUT_LIMTI 30

// includes
#include<sys/types.h>
#include<sys/wait.h>

// functions

/*!
    
	<summary>
        this function orders all of the children to send a message to their client
    </summary>

	<param name="pipes">the child pipe holder that will be used</param>
	<param name="msg">the message to be sent</param>

    <returns>void<returns>

*/
char* broadcast (int ** pipes, char * msg);