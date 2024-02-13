/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acharlot <acharlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 12:22:25 by axel              #+#    #+#             */
/*   Updated: 2024/02/13 13:42:13 by acharlot         ###   ########.fr       */
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

/**
 * @brief Adds a user to the list of kicked users in the channel.
 * 
 * This function checks if the specified user is already in the list of kicked users.
 * If not, it adds the user to the list and prints a message indicating the action.
 * 
 * @param kicked_name The name of the user to be kicked.
 */
void	Channel::addToKicked(std::string &kicked_name){
	std::vector<std::string>::iterator	it;
	for (it = _kicked_users.begin(); it != _kicked_users.end(); it++){
		if (*it == kicked_name){
			std::cout << ToColor("[Channel]", Colors::Red) << kicked_name 
				<< ToColor(" is already kicked from channel ", Colors::Red) 
				<< getName() << std::endl;
			return ;
		}
	}
	_kicked_users.push_back(kicked_name);
	std::cout << ToColor("[Channel]", Colors::Red) << kicked_name 
				<< ToColor(" is now kicked from channel ", Colors::Red) 
				<< getName() << std::endl;
}

/**
 * @brief Adds a user to the list of banned users in the channel.
 * 
 * This function checks if the specified user is already in the list of banned users.
 * If not, it adds the user to the list and prints a message indicating the action.
 * 
 * @param banned_name The name of the user to be banned.
 */
void	Channel::addToBanned(std::string &banned_name) {
	std::vector<std::string>::iterator	it;
	for (it = _banned_users.begin(); it != _banned_users.end(); it++){
		if (*it == banned_name){
			std::cout << ToColor("[Channel]", Colors::Red) << banned_name 
				<< ToColor(" is already kicked from channel ", Colors::Red) 
				<< getName() << std::endl;
			return ;
		}
		_banned_users.push_back(banned_name);
		std::cout << ToColor("[Channel]", Colors::Red) << banned_name 
				<< ToColor(" is now kicked from channel ", Colors::Red)
				<< getName() << std::endl;
	}
}

/**
 * @brief Removes a user from the list of banned users in the channel.
 * 
 * This function searches for the specified user in the list of banned users.
 * If found, it removes the user from the list and prints a message indicating
 * the action. If the user is not found, it prints a message indicating that
 * the user has never been banned from the channel.
 * 
 * @param banned_name The name of the user to be removed from the banned list.
 */
void	Channel::removeFromBanned(std::string &banned_name){
	std::vector<std::string>::iterator	user;
	for (user = _banned_users.begin(); user != _banned_users.end(); user++) {
		if (*user == banned_name) {
			_banned_users.erase(user);
			std::cout << ToColor("[Channel]", Colors::Red) << banned_name 
				<< ToColor(" is not banned anymore from channel ", Colors::Red) 
				<< getName() << std::endl;
			return ;
		}
	}
	std::cout << ToColor("[Channel]", Colors::Red) << banned_name 
				<< ToColor(" has never been banned from channel ", Colors::Red)
				<< getName() << std::endl;
}

/**
 * @brief Checks if a user is banned from the channel.
 * 
 * This function iterates through the list of banned users in the channel.
 * If the specified user is found in the list, it returns true indicating
 * that the user is banned. If the list is empty or if the user is not found,
 * it returns false indicating that the user is not banned.
 * 
 * @param banned_name The name of the user to check for banned status.
 * @return bool Returns true if the user is banned, otherwise false.
 */
bool	Channel::isBanned(std::string &banned_name){
	std::vector<std::string>::iterator	user;
	if (_banned_users.empty() == true)
		return (false);
	for (user = _banned_users.begin(); user != _banned_users.end(); user++) {
		if (*user == banned_name)
			return (true);
	}
	return (false);
}

/**
 * @brief Adds a user to the list of voiced users in the channel.
 * 
 * This function checks if the specified user is already in the list of voiced users.
 * If not, it adds the user to the list and prints a message indicating the action.
 * 
 * @param voiced_name The name of the user to be voiced.
 */
void	Channel::addToVoiced(std::string &voiced_name){
	std::vector<std::string>::iterator	it;
	for (it = _voiced_users.begin(); it != _voiced_users.end(); it++){
		if (*it == voiced_name){
			std::cout << ToColor("[Channel]", Colors::Red) << voiced_name 
				<< ToColor(" is already voiced from channel ", Colors::Red) 
				<< getName() << std::endl;
			return ;
		}
	}
	_voiced_users.push_back(voiced_name);
	std::cout << ToColor("[Channel]", Colors::Red) << voiced_name 
				<< ToColor(" is now voiced from channel ", Colors::Red)
				<< getName() << std::endl;
}

/**
 * @brief Removes a user from the list of voiced users in the channel.
 * 
 * This function searches for the specified user in the list of voiced users.
 * If found, it removes the user from the list and prints a message indicating
 * the action. If the user is not found, it prints a message indicating that
 * the user has never been voiced in the channel.
 * 
 * @param voiced_name The name of the user to be removed from the voiced list.
 */
void	Channel::removeFromVoiced(std::string &voiced_name)
{
	std::vector<std::string>::iterator user;
	for (user = _voiced_users.begin(); user != _voiced_users.end(); user++)
	{
		if (*user == voiced_name)
		{
			_voiced_users.erase(user);
			std::cout << ToColor("[Channel]", Colors::Red) << voiced_name 
				<< ToColor(" is not voiced from channel ", Colors::Red) 
				<< getName() << std::endl;
			return ;
		}
	}
	std::cout << ToColor("[Channel]", Colors::Red) << voiced_name 
				<< ToColor(" has never been voiced from channel ", Colors::Red)
				<< getName() << std::endl;
}

/**
 * @brief Checks if a user is voiced in the channel.
 * 
 * This function iterates through the list of voiced users in the channel.
 * If the specified user is found in the list, it returns true indicating
 * that the user is voiced. If the list is empty or if the user is not found,
 * it returns false indicating that the user is not voiced.
 * 
 * @param voiced_name The name of the user to check for voiced status.
 * @return bool Returns true if the user is voiced, otherwise false.
 */
bool	Channel::isVoiced(std::string &voiced_name)
{
	std::vector<std::string>::iterator user;
	if (_voiced_users.empty() == true) 
		return (false);
	for (user = _voiced_users.begin(); user != _voiced_users.end(); user++)
	{
		if (*user == voiced_name)
			return (true);
	}
	return (false);	
}

/**
 * @brief Adds the first operator to the channel.
 * 
 * This function adds the specified operator name to the list of operators
 * in the channel only if the list is currently empty.
 * 
 * @param operatorName The name of the first operator to add.
 */
void	Channel::addFirstOperator(std::string operatorName){
	if (_operators.empty())
		_operators.push_back(operatorName);
}

/**
 * @brief Removes an operator from the channel.
 * 
 * This function removes the specified operator name from the list of operators
 * in the channel if it exists.
 * 
 * @param operatorName The name of the operator to remove.
 */
void	Channel::removeOperator(std::string operatorName){
	std::vector<std::string>::iterator it;
	for (it = _operators.begin(); it != _operators.end(); it++){
		if (*it == operatorName)
			_operators.erase(it);
	}
}

/**
 * @brief Checks if a user is an operator in the channel.
 * 
 * This function iterates through the list of operators in the channel.
 * If the specified user is found in the list, it returns true indicating
 * that the user is an operator. If the list is empty or if the user is
 * not found, it returns false indicating that the user is not an operator.
 * 
 * @param operatorName The name of the user to check for operator status.
 * @return bool Returns true if the user is an operator, otherwise false.
 */
bool	Channel::isOperator(std::string &operatorName){
	std::vector<std::string>::iterator	user;
	if (_operators.empty())
		return (false);
	for (user = _operators.begin(); user != _operators.end(); user++){
		if (*user == operatorName)
			return (true);
	}
	return (false);
}

/**
 * @brief Adds a mode to the channel's mode string.
 * 
 * This function appends the specified mode to the channel's mode string.
 * If the mode string is currently empty, it initializes it with the mode
 * prefixed by a plus sign ('+'). If the mode string is not empty, it appends
 * the mode to the existing mode string.
 * 
 * @param mode The mode to add to the channel's mode string.
 */
void	Channel::addMode(std::string const mode){
	if (_mode.empty() == true)
		_mode = "+" + mode;
	else
		_mode += mode;
}

/**
 * @brief Removes a mode from the channel's mode string.
 * 
 * This function finds and removes the specified mode from the channel's mode string.
 * It searches for the mode in the mode string and erases the character at the found position.
 * 
 * @param mode The mode to remove from the channel's mode string.
 */
void	Channel::removeMode(std::string const mode){
	size_t pos = _mode.find(mode);
	_mode.erase(pos, 1);
}

/**
 * @brief Removes the channel's password.
 * 
 * This function clears the channel's password by emptying the channel_password string.
 */
void	Channel::removeChannelPassword(){
	_channel_password.clear();
}