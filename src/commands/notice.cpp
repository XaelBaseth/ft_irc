/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acharlot <acharlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:34:36 by acharlot          #+#    #+#             */
/*   Updated: 2024/02/19 15:34:37 by acharlot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/ircserv.hpp"

/**
 * *Prototype for the command 	=> `/notcie user`
 * *							=> `/notice #channel`
*/

/**
 * @brief Broadcasts a message to all members of a channel except the sender.
 * 
 * This function broadcasts a message to all members of the specified channel except
 * the sender. It first checks if the sender is not in the list of kicked users for
 * the channel. Then, it iterates through the list of members in the channel and sends
 * the message to each member using the addToClientBuffer function.
 * 
 * @param server Pointer to the server object.
 * @param client_fd File descriptor of the client sending the message.
 * @param it_client Iterator pointing to the client in the client list.
 * @param it_channel Iterator pointing to the channel in the channel list.
 * @param target The target channel name.
 * @param msg_to_send The message to send.
 */
static void  broadcastToChannel(Server *server, int const client_fd, std::map<const int, Client>::iterator it_client, std::map<std::string, Channel>::iterator it_channel, 
std::string target, std::string msg_to_send){
	std::vector<std::string> kicked_users = it_channel->second.getKickedUsers();

	for (std::vector<std::string>::iterator it = kicked_users.begin(); it != kicked_users.end(); it++){
		if (*it == it_client->second.getNickname())
			return ;
	}
	std::map<std::string, Client>::iterator member = it_channel->second.getClientList().begin();
	while (member != it_channel->second.getClientList().end()){
		addToClientBuffer(server, member->second.getClientFd(), RPL_NOTICE(it_client->second.getNickname(), it_client->second.getUsername(), target, msg_to_send));
	member++;
	}
}

/**
 * @brief Checks if a user is in the specified channel.
 * 
 * This function checks if the user associated with the provided client iterator
 * is in the specified channel associated with the provided channel iterator.
 * 
 * @param it_client Iterator pointing to the client in the client list.
 * @param it_channel Iterator pointing to the channel in the channel list.
 * @return true if the user is in the channel, false otherwise.
 */
static bool isUserinChannel(std::map<const int, Client>::iterator it_client, std::map<std::string, Channel>::iterator it_channel){
	if (it_channel->second.getClientList().find(it_client->second.getNickname()) != it_channel->second.getClientList().end())
		return (true);
	return (false);
	}

/**
 * @brief Handles the NOTICE command by sending a notice to a specified target.
 * 
 * This function parses the message to extract the target and message to send.
 * If the target is a channel, it broadcasts the message to all members of the channel.
 * If the target is a user, it sends the message directly to the user.
 * 
 * @param server Pointer to the server object.
 * @param client_fd File descriptor of the client sending the notice.
 * @param cmd_infos Structure containing command information.
 */
void	notice(Server *server, int const client_fd, s_cmd cmd_infos){ 
	std::map<const int, Client>				client_list = server->getClients();
	std::map<std::string, Channel>			channel_list = server->getChannels(); 
	std::map<const int, Client>::iterator	it_client = client_list.find(client_fd);
	std::string 							target;
	std::string								msg_to_send;

	size_t	delimiter = cmd_infos.message.rfind(":");
	if (delimiter == std::string::npos)
		return ;
	target = cmd_infos.message.substr(1, delimiter - 1);
	if (target.empty())
		return ;
	msg_to_send = cmd_infos.message.substr(delimiter);

	delimiter = target.find(" ");
	if (delimiter != std::string::npos)
		target.erase(delimiter);  

	if (target[0] == '#'){
		std::map<std::string, Channel>::iterator it_channel = channel_list.find(target.substr(1));

		if (it_channel == channel_list.end())
			return ;
		else
			broadcastToChannel(server, client_fd, it_client, it_channel, target, msg_to_send);
	}
	else {
		std::map<std::string, Channel>::iterator it_channel = channel_list.find(target);
		
		std::map<const int, Client>::iterator it_target = client_list.begin();
		while (it_target!=client_list.end())
		{
			if (it_target->second.getNickname() == target)
				break;
			it_target++;
		}
		if (it_target == client_list.end() && it_channel == channel_list.end())
			return ;  
		else{
			if (it_target == client_list.end()){
			if (isUserinChannel(it_client, it_channel) == true)
			{
				target.insert(1, "#");
				broadcastToChannel(server, client_fd, it_client, it_channel, target, msg_to_send);
			}
			else
				return ;
			}
			else
			addToClientBuffer(server, it_target->first, 
			RPL_NOTICE(it_client->second.getNickname(), it_client->second.getUsername(), target, msg_to_send));    
		}
	}
}