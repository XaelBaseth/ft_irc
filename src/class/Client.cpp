/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axel <axel@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 12:22:28 by axel              #+#    #+#             */
/*   Updated: 2024/02/10 12:22:29 by axel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/Client.hpp"

							/*****************************
							 * CONSTRUCTOR & DESTRUCTOR *
							*****************************/

Client::Client(int client_fd): _client_fd(client_fd - 3){
	/*std::cout << ToColor("Client constructor for Client #", Colors::Yellow)
		<< client_fd << std::endl;
	*/
}

Client::~Client(void){
}

							/********************
							 * GETTER & SETTER *
							*********************/

int				Client::getClientFd() const { return (_client_fd);}
std::string&	Client::getSendBuffer(){ return (_sendbuf);}
std::string&	Client::getReadBuffer(){ return (_readbuf);}

void	Client::setReadBuffer(std::string const &buf) {
	_readbuf += buf;
}

std::string&	Client::getNickname(){
	return (_nickname);
}

							/**********************
							 *		FUNCTIONS	 *
							***********************/