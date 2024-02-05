#include "../../lib/ircserv.hpp"

Server::Server(){
	std::cout << ToColor("Server running...", Colors::RoyalBlue) << std::endl;
	std::cout << ToColor("Server listening...", Colors::RoyalBlue) << std::endl;
}

Server::~Server(){
	std::cout << ToColor("Server is shutting down...", Colors::Gold) << std::endl;
}

const char * 	Server::InvalidClientException::what (void) const throw() 
{
	return "The information given by the client are invalid.";
}