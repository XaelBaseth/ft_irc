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

							/********************
							 * GETTER & SETTER *
							*********************/

std::map<const int, Client>&	Server::getClients(){
	return (_clients);
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
		std::cerr << ToColor("[Error] unable to create socket", Colors::Red) << std::cout;
		return (FAILURE);
	}
	
	int	optvalue = 1; //!enables the reuse of a port if the IP address is different.
	if (setsockopt(_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue)) == FAILURE) {
		std::cerr << ToColor("[Error] impossible to reuse", Colors::Red) << std::cout;
		return (FAILURE);
	}

	if (bind(_server_socket_fd, _servinfo->ai_addr, _servinfo->ai_addrlen) == FAILURE) {
		std::cerr << ToColor("[Error] impossible to bind", Colors::Red) << std::cout;
		return (FAILURE);
	}

	if (listen(_server_socket_fd, BACKLOG) == FAILURE) {
		std::cerr << ToColor("[Error] listen failed", Colors::Red) << std::cout;
		return (FAILURE);
	}
	freeaddrinfo(_servinfo);
	return (SUCCESS);
}

/**
 * Help to set up the structs `hints` and `servinfo`
 * @param port Value given by the user to connect.
 * @return `SUCCESS` or `FAILURE` wether getaddrinfo works or not.
*/
int	Server::fillInfos(char *port){
	if (getaddrinfo(NULL, port, &_hints, &_servinfo) < 0){
		std::cerr << ToColor("[Error] unable to use addrinfo", Colors::Red) << std::endl;
		return (FAILURE);
	}
	return (SUCCESS);
}

/**
 * Adds a new client to the server's poll file descriptor set and clients map.
 * 
 * Creates a new `pollfd` structure sor the client socket, set the events
 * to monitor for input (`POLLIN`) and output (`POLLOUT`) and adds the pollfd
 * the vector of polling file descriptors.
 * Adds a new `Client` object and inserts it into the `newClients` using the
 * client socket  as the key.
 * @param client_socket - The file descriptor of the client's socket connection.
 * @param poll_fds - a vector list referencing the `pollfd` structures 
 * representing the file descriptors of active connections.
*/
void	Server::addClient(int client_socket, std::vector<pollfd> &poll_fds)
{
	pollfd	client_pollfd;
	Client	newClient(client_socket);

	client_pollfd.fd = client_socket;
	client_pollfd.events = POLLIN | POLLOUT;
	poll_fds.push_back(client_pollfd);

	_clients.insert(std::pair<int, Client>(client_socket, newClient));
	std::cout << ToColor("[Server] Added client #", Colors::DeepPink) 
	<< client_socket << ToColor(" successfully", Colors::DeepPink) << std::endl;
}

/**
 * 	Delete a client from the existing client map managed by the server.
 * @param poll_fds - a vector list referencing the `pollfd` structures 
 * representing the file descriptors of active connections.
 * @param it - iterator pointing to the `pollfd` entry of client to be
 * deleted within the `poll_fds`
 * @param current_fd - file descriptor associated with the client to be deleted.
*/
void	Server::delClient(std::vector<pollfd> &poll_fds, 
	std::vector<pollfd>::iterator &it, int current_fd) {
	std::cout << ToColor("[Server] Deconnection of client #", Colors::DeepPink) 
	<< current_fd << std::endl;

	int key = current_fd;

	close(current_fd);
	_clients.erase(key);
	poll_fds.erase(it);

	std::cout << ToColor("[Server] Client deleted. Total client is now: ", Colors::DeepPink) << (unsigned int)(poll_fds.size() -1) << std::endl;
}