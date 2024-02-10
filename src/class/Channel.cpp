/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axel <axel@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 12:22:25 by axel              #+#    #+#             */
/*   Updated: 2024/02/10 12:26:04 by axel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/Channel.hpp"

							/*****************************
							 * CONSTRUCTOR & DESTRUCTOR *
							******************************/

Channel::Channel(std::string const &Name)
: _name(Name), _capacity_limit(-1){
	_banned_users.clear();
	_clientList.clear();
	_topic.clear();
}

Channel::~Channel(){

}

							/********************
							 * GETTER & SETTER *
							*********************/

std::string						&Channel::getName(){ return (_name);}
std::string						&Channel::getTopic(){ return (_topic);}
std::string						&Channel::getMode(){ return (_mode);}
std::string						&Channel::getChannelPassword(){ return (_channel_password);}
int								&Channel::getCapacityLimit(){ return (_capacity_limit);}
std::map<std::string, Client>	&Channel::getClientList(){ return (_clientList);}
std::vector<std::string>		&Channel::getBannedUsers(){ return (_banned_users);}
std::vector<std::string>		&Channel::getKickedUsers(){ return (_kicked_users);}
std::vector<std::string>		&Channel::getOperators(){ return (_operators);}

void	Channel::setTopic(std::string &newTopic){
	_topic = newTopic;
}

void	Channel::setChannelPassword(std::string password){
	_channel_password = password;
}

void	Channel::setCapacityLimit(int limit){
	_capacity_limit = limit;
}

							/**********************
							 *		FUNCTIONS	 *
							***********************/

/**
 * @brief Checks if a client exists in the channel's client list.
 * 
 * This function determines whether a client with the specified name exists
 * in the channel's client list. If the client list is empty, it returns false.
 * Otherwise, it searches for the client using its name in the client list map.
 * If the client is found, it returns true; otherwise, it returns false.
 * 
 * @param clientName The name of the client to check for existence.
 * @return bool Returns true if the client exists, otherwise false.
 */
bool	Channel::doesClientExist(std::string &clientName){
	if (_clientList.size() == 0)
		return (false);
	
	std::map<std::string, Client>::iterator it = _clientList.find(clientName);
	if (it == _clientList.end())
		return (false);
	return (true);
}

/**
 * @brief Adds a client to the channel's client list.
 * 
 * This function inserts a new client into the channel's client list using
 * the client's nickname as the key in the map.
 * 
 * @param client The client object to add to the channel.
 */
void	Channel::addClientToChannel(Client &client){
	_clientList.insert(std::pair<std::string, Client>(client.getNickname(), client));
}