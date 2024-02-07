#include "../../lib/ircserv.hpp"

/**
 * Constructor for the server class.
*/
Server::Server(){
	std::cout << ToColor("Server running...", Colors::RoyalBlue) << std::endl;
	std::cout << ToColor("Server listening...", Colors::RoyalBlue) << std::endl;
	memset(&_hints, 0, sizeof(_hints));
}

/**
 * Destructor for the server class.
*/
Server::~Server(void){
	std::cout << ToColor("Server is shutting down...", Colors::Gold) << std::endl;
}

/**
 * Error given when the port or password are wrong.
*/
const char * 	Server::InvalidClientException::what (void) const throw() 
{
	return "The credentials given are invalid.";
}

/**
 * Attributes the correct parameters to the hint structure to set up the server.
*/
void	Server::setHints(void) {
	_hints.ai_family = AF_INET;		//*IPV4 or IPV6
	_hints.ai_socktype = SOCK_STREAM;	//*TCP stream sockets
	_hints.ai_flags = AI_PASSIVE;		//*localhost default
}

/**
 * Launch the server with the differents socket functions.
 * 	
 *  1) socket() => get the server socket file descriptor.
 * 	2) setsocktop() => enable the configuration of said socket (here, we wanted
 * 							to allow the re-use of a port if the IP address is different)
 * 	3) bind() => Associate the socket with a specific port.
 * 	4) listen() => Wait for incoming connections.
 * 
 * @return int `SUCCESS` or `FAILURE`
*/
int	Server::launchServer(void){
	_server_socket_fd = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);
	if (_server_socket_fd == FAILURE) {
		std::cerr << ToColor("[Error] unable to create socket.", Colors::Red) << std::cout;
		return (FAILURE);
	}
	
	int	optvalue = 1; //!enables the reuse of a port if the IP address is different.
	if (setsockopt(_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue)) == FAILURE) {
		std::cerr << ToColor("[Error] impossible to reuse.", Colors::Red) << std::cout;
		return (FAILURE);
	}

	if (bind(_server_socket_fd, _servinfo->ai_addr, _servinfo->ai_addrlen) == FAILURE) {
		std::cerr << ToColor("[Error] impossible to bind.", Colors::Red) << std::cout;
		return (FAILURE);
	}

	if (listen(_server_socket_fd, BACKLOG) == FAILURE) {
		std::cerr << ToColor("[Error] listen failed.", Colors::Red) << std::cout;
		return (FAILURE);
	}
	freeaddrinfo(_servinfo);
	return (SUCCESS);
}

/**
 * Help to set up the structs `hints` and `servinfo`
 * @param port Value given by the user to connect.
 * @return int`SUCCESS` or `FAILURE` wether getaddrinfo works or not.
*/
int	Server::fillInfos(char *port){
	if (getaddrinfo(NULL, port, &_hints, &_servinfo) < 0){
		std::cerr << ToColor("[Error] unable to use addrinfo", Colors::Red) << std::endl;
		return (FAILURE);
	}
	return (SUCCESS);
}