#include "../../lib/ircserv.hpp"

/**
 * Main functions for the ircserv program.
 * TODO: Check if the input of the user is correct.
 * TODO: Start the server.
 * ?TODO: Handle sigint properly?
 * TODO: Loop for the waiting and listening. 
 * ?^ try catch ^?
*/
int	main(int argc, char **argv) {
	if (argc != 3){
		std::cerr << ToColor("[Usage] ./ircserv <port> <password>.", Colors::DarkMagenta) << std::endl;
		return (FAILURE);
	}
	else {
		Server	server;

		server.setHints();
		if (server.fillInfos(argv[1]) == FAILURE)
			return (FAILURE);
		server.launchServer();
		return (SUCCESS);
	}
}