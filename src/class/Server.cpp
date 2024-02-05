#include "../../lib/ircserv.hpp"

Server::Server(){
	std::cout << ToColor("Server has been created.", Colors::RoyalBlue) << std::endl;
}

Server::~Server(){
	std::cout << ToColor("Server has been destroyed.", Colors::Gold) << std::endl;
}