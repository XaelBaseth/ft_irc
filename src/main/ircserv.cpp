#include "../../lib/ircserv.hpp"

/**
 * Handle the quitting of the server properly.
 * @param signal - signal that needs to be handled.
*/
static void	signal_handler(int signal){
	(void)signal;
	std::cout << "\033[2D"; //hide the ^C in the terminal
	server_shutdown = true;
}

/**
 * Main functions for the ircserv program.
 * TODO: Check if the input of the user is correct.
*/
int	main(int argc, char **argv) {
	if (argc != 3){
		std::cerr << ToColor("[Usage] ./ircserv <port> <password>.", Colors::DarkMagenta) << std::endl;
		return (FAILURE);
	}
	else {
		signal(SIGINT, signal_handler);
		Server	server;

		server.setHints();
		if (server.fillInfos(argv[1]) == FAILURE)
			return (FAILURE);
		server.launchServer();
		try {
			server.manageServerLoop();
		}
		catch(const std::exception& e){
			std::cerr << ToColor("Caught exception: ", Colors::Red)
				<< e.what() << std::endl;
		}
		return (SUCCESS);

	}
}