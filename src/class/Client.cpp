#include "../../lib/Client.hpp"
 /**
  * Constructor of the client class.
 */
Client::Client(int client_fd): _client_fd(client_fd){
	/*std::cout << ToColor("Client constructor for Client #", Colors::Yellow)
		<< client_fd << std::endl;
	*/
}

/**
 * Destructor of the client class.
*/
Client::~Client(void){
	
}

							/********************
							 * GETTER & SETTER *
							*********************/

int				Client::getClientFd() const {
	 return (_client_fd); 
}

std::string&	Client::getSendBuffer(){
	return (_sendbuf);
}

std::string&	Client::getReadBuffer(){
	return (_readbuf);
}

void	Client::setReadBuffer(std::string const &buf) {
	_readbuf += buf;
}