// own includes
#include "shared_includes.h"
#include "server_includes.h"

// main program loop
int main(int argc, char* const argv[]) {
	
	// introduce program
	printf("Welcome to S5132483's game_server program for 2803ICT Assignment 2\n");

	// is there an incorrect number of arguments
	if (argc != 4) {

		printf("ERROR: you must use the program correctly launching with the right ammount of arguments\n");
		printf("correct usage is as follows:\n./game_server.exe <port number> <game type> <game arguments>\n");
		exit(1);

	// possible mode for dry start
	} else if (argc == 1) {

		exit(1);

	}

	// check that game type is numbers
	if (strcmp(argv[2], "numbers")) {
	
		printf("ERROR: game type not set to 'numbers', this is currently the only game which can be played on this client\n");
		exit(1);

	}

	// initialise variables
	/*!
    client_buffer = a string of used as a buffer for input sent by the user
	*/ 
	char * client_buffer;
	/*!
    server_buffer = a string of used as a buffer for input sent by the server
	*/ 
	char * server_buffer;
	/*!
    server_address_info = a string of constant length used to store the address that the server is hosting on
	*/ 
	char server_address_info[INET_ADDRSTRLEN];
	/*!
	args = a holder for any args that may be given along with a command
	*/ 
	char * args;
	/*!
    server_socket_id = the id of the server socket being used
	*/ 
	int server_socket_id;
	/*!
    server_socket_id = the id of the client socket being used
	*/ 
	int client_socket_id;
	/*!
    server = a struct containing all of the relevant details on the servers connectivity
	*/ 
	struct sockaddr_in server;
	/*!
    client = a struct containing all of the relevant details on communication to the clients
	*/ 
	struct sockaddr_in client;
	/*!
    result = an integer which will be used to indicate the status of connction attempts
	*/ 
	int result;
	/*!
    game_status = an integer which will be used as a flag to indicate the games current status
	*/ 
	int game_status = 0;
	/*!
    max_player_count = an integer which contains the maximum number of players allowed in this game
	*/ 
	int max_player_count;
	/*!
    current_player_count = an integer which contains the maximum number of players allowed in this game
	*/ 
	int current_player_count = 0;
	/*!
    start = a pointer for the time since epooch when an action was attempted
	*/ 
	time_t start;
	/*!
	end = a pointer for the time since epooch when an action was completed
	*/ 
	time_t end;
	/*!
	cpid = a pointer for the time since epooch when an action was completed
	*/ 
	pid_t cpid;
	/*!
	pipe_child_orders = an array of pipes, one for each player allowing the parent to comunicate
	*/ 
	int ** pipe_child_orders;
	/*!
	players = an array of keeping track of which players are still in the game
	*/ 
	int * players;
	/*!
	pipe_moves = a pipe used to convey the moves from the child processes to the parent process
	*/ 
	int pipe_moves[2];
	/*!
	round_count = keeps track of the ammount of rounds that have occured
	*/ 
	int round_count = 1;
	/*!
	score = the current score of the game
	*/ 
	int score = 0;

	// get the max player count
	max_player_count = atoi(argv[3]);
	
	// open the moves pipe
	result = pipe(pipe_moves);
	
	//check moves pipe
	if (result < 0) {
		
		myTime();
		printf(" - ERROR: could create moves pipe\n");
		exit(1);
	
	}

	// announce status
	myTime();
	printf(" - moves pipe created\n");

	// allocate pipes array and players
	pipe_child_orders = (int **)calloc(max_player_count, sizeof(int *));
	players = (int *)calloc(max_player_count, sizeof(int));

	// allocate, create and check the order pipes
	for (int x=0; x < max_player_count; x++) {

		players[x] = 1;

		// allocate
		pipe_child_orders[x] = (int *)calloc(2, sizeof(int));

		// open the moves pipe
		result = pipe(pipe_child_orders[x]);
		
		//check moves pipe
		if (result < 0) {
			
			myTime();
			printf(" - ERROR: could not create order pipe#%d\n", x);
			exit(1);
		
		}

		// announce status
		myTime();
		printf(" - order pipe#%d created\n", x);

	}

	// configure connection
	server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

	// open TCP socket
	server_socket_id = socket(AF_INET, SOCK_STREAM, 0);
	
	// check the socket
	if (server_socket_id < 0) {

		myTime();
		printf(" - ERROR: could not create socket\n");
		exit(1);

	}

	// announce status
	myTime();
    printf(" - socket created\n");

	// bind the address to the 
	result = bind(server_socket_id, (struct sockaddr * ) & server, sizeof(server));

	// check the bind
	if (result < 0) {

		myTime();
		printf(" - ERROR: Could not bind the socket\n");
		close(server_socket_id);
		exit(1);

	}

	// announce status
	myTime();
    printf(" - socket binded\n");

	// check for valid server details and display them to console
	if (inet_ntop(AF_INET, & server.sin_addr.s_addr, server_address_info, sizeof(server_address_info)) == NULL) {

		myTime();
		printf(" - ERROR: server details could not be found\n");
		close(server_socket_id);
		exit(1);

	}

	// announce hosting details
	myTime();
    printf(" - server hosting on: %s port: %d\n", server_address_info, ntohs(server.sin_port));

	// setup listen
	result  = listen(server_socket_id, BACK_LOG);

	// check listen
	if (result < 0) {

		myTime();
		printf(" - ERROR: Could not successfully listen\n");
		close(server_socket_id);
		exit(1);

	}

	// announce status
	myTime();
    printf(" - listen successfully opened\n");

	// allocate the buffers
	client_buffer = (char *)calloc(MAXIN, sizeof(char));
	server_buffer = (char *)calloc(MAXIN, sizeof(char));

	fflush(stdout);

	// accept multiple connections and fork into child processes
	// until max player count is reached
	while (current_player_count < max_player_count) {


		int tmp_len = sizeof(client);

		// make socket connection
		client_socket_id = accept(server_socket_id, (struct sockaddr *) & client, & tmp_len);

		// setup timeout
		struct timeval tv;
		tv.tv_sec = TIMEOUT_LIMTI;
		tv.tv_usec = 0;
		setsockopt(client_socket_id, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

		// check the socket
		if (server_socket_id < 0) {

			myTime();
			printf(" - ERROR: could not create socket\n");
			exit(1);

		} 
	
		// fork
		cpid = fork();

		// check fork
		if (cpid < 0) {

			myTime();
			printf(" - ERROR: could not fork\n");

		// if the fork was successful then increase the player count and break out the child 
		} else {


			// parent 
			if (cpid > 0) {

				// announce status
				myTime();
				printf(" - a new player has joined\n", cpid);
				current_player_count++;
				
			}

			// break if this is a child process
			if (cpid == 0)
				break;

		}
	
	}

	// child loop	
	if (cpid == 0) {
		
		// tell the client about the game
		// make message
		strcpy(server_buffer, "TEXT hello player, welcome to the game, it will begin when the lobby is full");
		// send message
		send(client_socket_id, server_buffer, MAXIN, 0);
		// clear buffer
		memset(server_buffer, '\0', MAXIN);

		int move, errors;

		// main client loop
		while (1) {

			// if an order has been given then process the order
			if (read(pipe_child_orders[current_player_count][0], server_buffer, MAXIN) != 0) {

				// GO
				if (!strcmp(server_buffer, "GO")) {
					
					errors = 0;

					while (1) {

						// send them the command
						send(client_socket_id, "GO", MAXIN, 0);

						result = 0;

						// refresh buffers
						memset(server_buffer, '\0', MAXIN);
						memset(client_buffer, '\0', MAXIN);
						memset(args, '\0', MAXIN);

						// wait for a response
						result = recv(client_socket_id, client_buffer, MAXIN, 0);

						// if theres a response process it
						if (result > 0) {

							// use received data
							args = popFront(client_buffer);

							// MOVE
							if (!strcmp(client_buffer, "MOVE")) {

								// get the number
								move = atoi(args);

								// if this move is valid then send it back to the parent
								if ((1 <= move) && (move <= 9)) {

									// make message
									sprintf(server_buffer, "%d", move);
									// send
									write(pipe_moves[1], server_buffer, strlen(server_buffer));

									break;

								// this move is invalid
								} else {

									// make error message
									sprintf(server_buffer, "TEXT ERROR given number is invalid, correct usage is: MOVE <number between 1 and 9>");
									// send error message
									send(client_socket_id, server_buffer, MAXIN, 0);

									if (errors < 4){

										errors++;
										continue;

									}

								}

							// if not QUIT, then unknown command
							} else if (strcmp(client_buffer, "QUIT")) {

								// make error message
								sprintf(server_buffer, "ERROR unknown command");
								// send error message
								send(client_socket_id, server_buffer, MAXIN, 0);

							}

						}

						// if the code has gotten here then 1 of a few things have happend
						// the client timed out, the command was invalid, or the client sent quit

						// refresh buffers
						memset(server_buffer, '\0', MAXIN);
						memset(client_buffer, '\0', MAXIN);

						// tell client to disconnect
						// make message
						strcpy(server_buffer, "END");
						// send message
						send(client_socket_id, server_buffer, MAXIN, 0);

						// send leave signal to parent
						// make message
						strcpy(client_buffer, "-1");
						write(pipe_moves[1], client_buffer, strlen(client_buffer));

						// close the connection
						close(client_socket_id);

						// die
						exit(0);

					}

					continue;

				}

				// END
				if (!strcmp(server_buffer, "END")){

					// send the message to the client
					send(client_socket_id, server_buffer, MAXIN, 0);

					// close the connection
					close(client_socket_id);

					// die
					exit(0);

				}

				// for text or error
				send(client_socket_id, server_buffer, MAXIN, 0);

				// clear buffer
				memset(server_buffer, '\0', MAXIN);

			}

		}
	
	// parent loop
	} else {

		// announce status
		myTime();
		printf(" - player capacity reached, the game is beginning\n");

		// announce status to players
		// make message
		strcpy(server_buffer, "TEXT all players have joined, begining the game");
		// send message
		for (int x=0; x < max_player_count; x++)
			if (players[x] == 1) {

				write(pipe_child_orders[x][1], server_buffer, strlen(server_buffer));

			}

		// beginning game loop
		while (1) {

			// cycle through the players
			for (int x=0; x < max_player_count; x++) {

				// if the player count falls to 1 then end the game
				if (current_player_count < 2) {
				
					// clear buffer
					memset(server_buffer, '\0', MAXIN);
				
					// announce end due to low player count
					// make message
					sprintf(server_buffer, "TEXT game over due to lack of players");
					// send message
					for (int x=0; x < max_player_count; x++)
						if (players[x] == 1)
							write(pipe_child_orders[x][1], server_buffer, strlen(server_buffer));

					// give the message some time
					sleep(5);

					// refresh buffer
					memset(server_buffer, '\0', MAXIN);	

					// end due to low player count
					// make message
					sprintf(server_buffer, "END");
					// send message
					for (int x=0; x < max_player_count; x++)
						if (players[x] == 1)
							write(pipe_child_orders[x][1], server_buffer, strlen(server_buffer));

					// announce to console
					printf("game over due to lack of players\n");

					current_player_count--;

					// break the loop
					break;

				}

				if (players[x] == 1) {
					
					// declare current player
					myTime();
					printf(" - beginning player %d's go\n", (x+1));

					// order the child process to get a response from the player
					// make order 
					strcpy(server_buffer, "GO");
					// send message
					write(pipe_child_orders[x][1], server_buffer, strlen(server_buffer));

					// wait for the child to get the players response and send back the score
					while (1) {
					
						// if the child has signaled a response by the player
						if (read(pipe_moves[0], server_buffer, MAXIN) != 0) {

							// did the player leave the game
							if (!strcmp(server_buffer, "-1")) {

								// announce status
								myTime();
								printf(" - player %d has left the game\n", (x + 1));

								// set player as left
								players[x] = 0;

								// tell child to close
								// make message
								strcpy(server_buffer, "END");
								// send message
								write(pipe_child_orders[x][1], server_buffer, strlen(server_buffer));

								// construct move message
								sprintf(server_buffer, "TEXT player %d has left the game", (x + 1));
								
								current_player_count--;

							} else {

								score += atoi(server_buffer);

								// announce status
								myTime();
								printf(" - player %d has played %d, bringing the total to %d\n", (x + 1), atoi(server_buffer), score);

								// construct move message
								sprintf(server_buffer, "TEXT player %d played %d, the total is now %d", (x + 1), atoi(server_buffer), score);

							}

							// send message to players
							for (int x=0; x < max_player_count; x++)
								if (players[x] == 1) {

									write(pipe_child_orders[x][1], server_buffer, strlen(server_buffer));

								}

							// if there is a winner announce the winner and end the game
							if (score >= GAME_GOAL) {

								// announce win
								// make message
								sprintf(server_buffer, "TEXT player %d has won", (x + 1));
								// send message
								for (int x=0; x < max_player_count; x++)
									if (players[x] == 1) {

										write(pipe_child_orders[x][1], server_buffer, strlen(server_buffer));

									}

								sleep(5);

								// announce end
								// make message
								strcpy(server_buffer, "END");
								// send message
								for (int x=0; x < max_player_count; x++)
									if (players[x] == 1) {

										write(pipe_child_orders[x][1], server_buffer, strlen(server_buffer));

									}

								// announce to console
								printf("game over due to lack of players\n");

								// handle deallocation and closing sequences
								close(server_socket_id);
								close(client_socket_id);

								// wait for children to die
								wait(NULL);

								// exit with no error
								exit(0);

							}

							break;

						}

					}

				}

			}

			if (current_player_count < 1)
				break;

		}
	
	}

	// handle deallocation and closing sequences
	close(server_socket_id);

	wait(NULL);

	// return program complete
	return 0;

}