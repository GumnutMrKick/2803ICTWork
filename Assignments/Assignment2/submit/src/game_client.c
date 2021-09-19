// own includes
#include "shared_includes.h"

// main program loop
int main(int argc, char* argv[]) {
	
	// introduce program
	printf("Welcome to S5132483's game_client program for 2803ICT Assignment 2\n");

	// is there an incorrect number of arguments
	if (argc != 4) {

		printf("ERROR: you must use the program correctly launching with the right ammount of arguments\n");
		printf("correct usage is as follows:\n./game_client.exe <game type> <server ip> <port number>\n");
		exit(1);

	// possible mode for dry start
	} else if (argc == 1) {

		exit(1);

	}

	// check that game type is numbers
	if (strcmp(argv[1], "numbers")) {
	
		printf("ERROR: game type not set to 'numbers', this is currently the only game which can be played on this client\n");
		exit(1);

	}

	// initialise variables
	/*!
    client_buffer = a string used as a buffer for input sent by the user
	*/ 
	char * client_buffer;
	/*!
    server_buffer = a string used as a buffer for input sent by the server
	*/ 
	char * server_buffer;
	/*!
	args = a holder for any args that may be given along with a command
	*/ 
	char * args;
	/*!
    server = a struct containing all of the relevant details on the address
	*/ 
	struct sockaddr_in server;
	/*!
    socket_id = the id of the socket being used
	*/ 
	int socket_id;
	/*!
    result = an integer which will be used to indicate the status of connction attempts
	*/ 
	int result;
	/*!
    quit = an integer which acts as a flag remembering if the client desires to quit
	*/ 
	int quit = 0;
	/*!
    start = a pointer for the time since epooch when an action was attempted
	*/ 
	time_t start;
	/*!
	end = a pointer for the time since epooch when an action was completed
	*/ 
	time_t end;

	// configure connection
	server.sin_addr.s_addr = inet_addr(argv[2]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[3]));

	// create socket
    socket_id = socket(AF_INET, SOCK_STREAM, 0);

	// check socket
    if (socket_id < 0) {
        
		myTime();
		printf(" - ERROR: Could not create socket\n");
		exit(1);

    }

	// announce status
	myTime();
    printf(" - socket created\n");

	// open the connection
	result = connect(socket_id, (struct sockaddr *) &server, sizeof(server));
    
	if (result < 0) {
        
		myTime();
		printf(" - ERROR: could not make a connection to the server\n");
        exit(1);
    
	}

	// announce status
	myTime();
    printf(" - connection opened\n");

	// allocate the buffers
	client_buffer = (char*)calloc(MAXIN, sizeof(char));
	server_buffer = (char*)calloc(MAXIN, sizeof(char));

	// announce listening
	myTime();
	printf(" - listening for server\n");

	// looped client enviroment
	while (1) {

		// refresh buffer
		memset(server_buffer, '\0', MAXIN);

		recv(socket_id, server_buffer, MAXIN, 0);

		// use received data
		args = popFront(server_buffer);

		// TEXT
		if (!strcmp(server_buffer, "TEXT")) {
			
			// test for args
			if (args[0] == '\0'){

				myTime();
				printf(" - ERROR: no arguments have been supplied with the TEXT command please contact the server\n");
				// disconnect

			}

			myTime();
			printf(" - server: %s\n", args);

			continue;

		}

		// GO
		if (!strcmp(server_buffer, "GO")) {
			
			// refresh buffer
			memset(client_buffer, '\0', MAXIN);	

			// inform the user of their turn
			myTime();
			printf(" - server: please take your turn:\n");
			
			// print cursor
			myTime();
			printf(" - $> ");

			// take user input, and clean it
			fgets(client_buffer, (MAXIN - 1), stdin);
			fgetsClean(client_buffer);

			// seperate args
			args = popFront(client_buffer);

			// if the input isn't quit or move
			if (!(!strcmp(client_buffer, "QUIT") || !strcmp(client_buffer, "MOVE"))) {

				// unpop the string
				unPopFront(client_buffer);

				result = atoi(client_buffer);

				// assume it is a commandless move and add MOVE prefix	
				// if the value given is a number
				if (result > 0)
					sprintf(client_buffer, "%s%d", "MOVE ", result);

			} else {

				// unpop the string
				unPopFront(client_buffer);

			}

			// send the command
			send(socket_id, client_buffer, MAXIN, 0);

		}

		// END
		if (!strcmp(server_buffer, "END")) {
			
			// this is expected
			if (quit == 1)
				printf("you have left the game\n");
			// this is not expected
			else 
				printf("the game has ended");

			// break the loop
			break;

		}

		// ERROR
		if (!strcmp(server_buffer, "ERROR")) {
			
			// test for args
			if (args[0] == '\0'){

				myTime();
				printf(" - ERROR: the server has indicated that and error has occured\n");
				continue;

			}

			myTime();
			printf(" - server ERROR: %s\n", args);

			continue;

		}

		result = 0;

	}

	// handle the deallocation and closing sequences
	close(socket_id);

	// return program complete
	return 0;

}