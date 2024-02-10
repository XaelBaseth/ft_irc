/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axel <axel@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 12:22:32 by axel              #+#    #+#             */
/*   Updated: 2024/02/10 12:27:17 by axel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/Server.hpp"

							/*****************************
							 * CONSTRUCTOR & DESTRUCTOR *
							******************************/
Server::Server(std::string port, std::string password, struct tm *timeinfo)
: _servinfo(NULL), _server_socket_fd(0), _port(port), _password(password){
	std::cout << ToColor("Server running...", Colors::Violet) << std::endl;
	std::cout << ToColor("Server listening...", Colors::Violet) << std::endl;
	memset(&_hints, 0, sizeof(_hints));
	this->setDatetime(timeinfo);
}

Server::~Server(void){
	std::cout << ToColor("Server is shutting down...", Colors::Violet) << std::endl;
}

							/********************
							 * GETTER & SETTER *
							*********************/

std::string						Server::getPort() const{ return (_port);}
std::string						Server::getPassword() const{ return (_password);}
std::string						Server::getDatetime() const{ return (_datetime);}
std::map<std::string, Channel>&	Server::getChannels(){ return (_channels);}
std::map<const int, Client>&	Server::getClients(){ return (_clients);}
std::vector<server_op>&		Server::getIRCOperators(){ return (_irc_operators); }
std::string						Server::getMotd() const{ return (_motd);}
void							Server::setPassword(std::string new_pwd){ _password = new_pwd;}

void							Server::setDatetime(struct tm *timeinfo){ 
	char	buffer[80];
	strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S", timeinfo);
	std::string str(buffer);

	_datetime = str;
}

void							Server::setMotd(std::string buffer){ 
	_motd = buffer;
}

/**
 * Error given when the port or password are wrong.
*/
const char * 	Server::InvalidClientException::what (void) const throw() 
{
	return "The credentials given are invalid.";
}

							/**********************
							 *		FUNCTIONS	 *
							***********************/

/**
 * @brief Sets up hints for address resolution.
 * 
 * This function initializes the hints structure used for address resolution
 * using the getaddrinfo() function. It sets the address family to IPv4 or IPv6,
 * socket type to TCP stream sockets, and flags to indicate passive socket
 * for localhost default behavior.
*/
void	Server::setHints(void) {
	_hints.ai_family = AF_INET;		//*IPV4 or IPV6
	_hints.ai_socktype = SOCK_STREAM;	//*TCP stream sockets
	_hints.ai_flags = AI_PASSIVE;		//*localhost default
}

/**
 * @brief Launches the server by creating and initializing the server socket.
 * 
 * This function creates a socket using the address family, socket type, and
 * protocol information stored in _servinfo. It sets the socket options to
 * allow reusing the port if the IP address changes. Then it binds the socket
 * to the specified address and port, and starts listening for incoming connections.
 * 
 * @return int Returns SUCCESS if the server is successfully launched, otherwise FAILURE.
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
 * @brief Fills server information using getaddrinfo.
 * 
 * This function fills the server information structure _servinfo by using the
 * getaddrinfo() function to retrieve address information for the specified port.
 * If the function fails, it prints an error message and returns FAILURE; otherwise,
 * it returns SUCCESS.
 * 
 * @param port The port number as a string.
 * @return int Returns SUCCESS if the information is successfully filled, otherwise FAILURE.
*/
int	Server::fillInfos(char *port){
	if (getaddrinfo(NULL, port, &_hints, &_servinfo) < 0){
		std::cerr << ToColor("[Error] unable to use addrinfo", Colors::Red) << std::endl;
		return (FAILURE);
	}
	return (SUCCESS);
}

/**
 * @brief Adds a new client to the server's client list and poll vector.
 * 
 * This function creates a new pollfd structure for the client socket,
 * configures it for both input and output events, and adds it to the
 * provided poll vector. It also creates a new Client object for the client
 * socket and adds it to the server's client list. Finally, it prints a
 * success message indicating the addition of the client.
 * 
 * @param client_socket The file descriptor of the new client socket.
 * @param poll_fds The vector of pollfd structures representing active file descriptors.
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
 * @brief Deletes a client from the server's client list and poll vector.
 * 
 * This function closes the client socket, removes the corresponding Client object
 * from the server's client list, and erases the pollfd structure associated with
 * the client socket from the poll vector. It also prints a message indicating the
 * disconnection of the client and the updated total number of clients.
 * 
 * @param poll_fds The vector of pollfd structures representing active file descriptors.
 * @param it An iterator pointing to the pollfd structure of the client socket.
 * @param current_fd The file descriptor of the client socket to delete.
*/
void	Server::delClient(std::vector<pollfd> &poll_fds, 
	std::vector<pollfd>::iterator &it, int current_fd) {
	std::cout << ToColor("[Server] Deconnection of client #", Colors::DeepPink) 
	<< current_fd << std::endl;

	int key = current_fd;

	close(current_fd);
	_clients.erase(key);
	poll_fds.erase(it);

	std::cout << ToColor("[Server] Client deleted. Total client is now: ", Colors::DeepPink) 
		<< (unsigned int)(poll_fds.size() -1) << std::endl;
}


/**
 * @brief Adds a new channel to the server's channel list.
 * 
 * This function checks if a channel with the specified name already exists
 * in the server's channel list. If not, it creates a new channel object with
 * the provided name and inserts it into the channel list.
 * 
 * @param channelName The name of the channel to add.
 */
void	Server::addChannel(std::string &channelName){
	std::map<std::string, Channel>::iterator it = _channels.find(channelName);
	if (it != _channels.end()){
		std::cout << ToColor("[Channel] name already exists.", Colors::Yellow) << std::endl;
		return ;
	}
	Channel channel(channelName);
	_channels.insert(std::pair<std::string, Channel>(channel.getName(), channel));
}

/**
 * @brief Adds a client to the specified channel.
 * 
 * This function checks if the client is already a member of the channel.
 * If not, it adds the client to the channel's client list and prints a
 * success message. If the client is already in the channel, it prints
 * a message indicating that the client is already present.
 * 
 * @param channelName The name of the channel to add the client to.
 * @param client The client object to add to the channel.
 */

void	Server::addClientToChannel(std::string &channelName, Client &client){
	std::map<std::string, Channel>::iterator it;
	it = _channels.find(channelName);
	
	std::string	client_nickname = client.getNickname();
	
	if (it->second.doesClientExist(client_nickname) == false){
		it->second.addClientToChannel(client);
		std::cout << ToColor("[Channel] successfully joined the channel", Colors::Yellow) << std::endl;
	}
	else
		std::cout << ToColor("[Channel] ", Colors::Yellow) << client.getNickname() 
			<< ToColor(" already here!", Colors::Yellow) << std::endl;
}

