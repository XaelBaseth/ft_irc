#include "../../lib/ircserv.hpp"

/**
 * @brief Bans a client from a channel.
 * 
 * This function bans a client from a channel if they exist in the channel and are not already banned.
 * 
 * @param server Pointer to the Server object.
 * @param client_fd File descriptor of the client requesting the ban operation.
 * @param channelName Name of the channel.
 * @param client_nickname Nickname of the client to be banned.
 * @param operator_nickname Nickname of the operator requesting the ban operation.
 */
static void	banClientFromChannel(Server *server, int const client_fd, std::string &channelName, 
std::string client_nickname, std::string operator_nickname){
	std::map<std::string, Channel>::iterator it = server->getChannels().find(channelName);
	if (it->second.doesClientExist(client_nickname) == true){
		if (it->second.isBanned(client_nickname) == false){
			it->second.addToBanned(client_nickname);

			std::map<const int, Client>::iterator it_client = server->getClients().find(client_fd);
			std::string	RPL_BAN = ":" + operator_nickname + "!" + it_client->second.getUsername()
			+ "@localhost MODE #" + channelName + " +b " + client_nickname + "\r\n";
			broadcastToAllChannelMembers(server, it->second, RPL_BAN);
			if (it->second.getMode().find("b") == std::string::npos)
				it->second.addMode("b");
		}
		else
			std::cout << YELLOW << client_nickname << " is already banned\n" << RESET;
	}
	else
		addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(client_nickname, channelName));
}

/**
 * @brief Removes a client from the ban list of a channel.
 * 
 * This function removes a client from the ban list of a channel if they are banned.
 * 
 * @param server Pointer to the Server object.
 * @param client_fd File descriptor of the client requesting the unban operation.
 * @param channelName Name of the channel.
 * @param client_nickname Nickname of the client to be unbanned.
 * @param operator_nickname Nickname of the operator requesting the unban operation.
 */
static void	unbanClientFromChannel(Server *server, int const client_fd, std::string &channelName, 
std::string client_nickname, std::string operator_nickname){
	std::map<const int, Client>::iterator it = server->getClients().begin();
	for (it = server->getClients().begin(); it != server->getClients().end(); it++){
		if (it->second.getNickname() == client_nickname)
			break ;
	}
	if (it != server->getClients().end()){
		std::map<std::string, Channel>::iterator it_channel = server->getChannels().find(channelName);
		if (it_channel->second.isBanned(client_nickname) == true){
			it_channel->second.removeFromBanned(client_nickname);
			std::map<const int, Client>::iterator it_client = server->getClients().find(client_fd);
			std::string	RPL_BAN = ":" + operator_nickname + "!" + it_client->second.getUsername()
			+ "@localhost MODE #" + channelName + " -b " + client_nickname + "\r\n";
			
			broadcastToAllChannelMembers(server, it_channel->second, RPL_BAN); 
		}
	}
	else
		addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(client_nickname, channelName));
}

/**
 * @brief Handles the ban mode for a channel.
 * 
 * This function manages the ban mode for a channel by either adding or removing bans.
 * 
 * @param server Pointer to the Server object.
 * @param datas Array containing mode change data: [0] nickname of the requester, 
 * 				[1] channel name, [2] mode change (+b or -b), [3] ban mask.
 * @param client_fd File descriptor of the client initiating the mode change.
 */
void	banChannelMode(Server *server, std::string datas[4], int const client_fd){
	if (server->isChannel(datas[1]) == false){
		addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(datas[0], datas[1]));
		return ;
	}

	if (datas[3].empty() == true){
		std::map<std::string, Channel>::iterator it_channel_target = server->getChannels().find(datas[1]);
		size_t pos = it_channel_target->second.getMode().find("b");
		if (datas[2][0] == '+'){
			if (pos != std::string::npos)
				return;
			it_channel_target->second.addMode("b");
			broadcastToAllChannelMembers(server, it_channel_target->second, MODE_CHANNELMSG(datas[1], "+b"));
		}
		else{
			if (pos == std::string::npos)
				return;
			it_channel_target->second.removeMode("b");
			broadcastToAllChannelMembers(server, it_channel_target->second, MODE_CHANNELMSG(datas[1], "-b"));
		}
		return ;
	}
	if (datas[2][0] == '+')
		banClientFromChannel(server, client_fd, datas[1], datas[3], datas[0]);
	else if (datas[2][0] == '-')
		unbanClientFromChannel(server, client_fd, datas[1], datas[3], datas[0]);
}