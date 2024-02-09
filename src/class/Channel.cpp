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

bool	Channel::doesClientExist(std::string &clientName){
	if (_clientList.size() == 0)
		return (false);
	
	std::map<std::string, Client>::iterator it = _clientList.find(clientName);
	if (it == _clientList.end())
		return (false);
	return (true);
}

void	Channel::addClientToChannel(Client &client){
	_clientList.insert(std::pair<std::string, Client>(client.getNickname(), client));
}