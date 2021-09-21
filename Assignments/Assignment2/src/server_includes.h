/*!
    BACK_LOG = a global constant contains as an integer of the maximum number of clients that can attempt to connect at the same time
*/ 
#define BACK_LOG 10

/*!
    GAME_GOAL = a global constant contains as an integer of the goal number to be reached in the game
*/ 
#define GAME_GOAL 30

/*!
    TIMEOUT_LIMIT = the ammount of time the server will wait for a client answer after giving them the go command
*/ 
#define TIMEOUT_LIMIT 30

// includes
#include<sys/types.h>
#include<sys/wait.h>

// functions
