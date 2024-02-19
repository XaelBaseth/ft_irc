#include "../../lib/ircserv.hpp"

/**
 * *Prototype for the command => `/list`
*/

/**
 * @brief Finds the name of a channel in a given message.
 * 
 * This function extracts the name of a channel from a given message. It searches for 
 * the first occurrence of a channel name starting with '#' and returns it. If no channel
 * name is found, an empty string is returned.
 * 
 * @param msg_to_parse The message to parse for a channel name.
 * @return The name of the channel found in the message, or an empty string if not found.
 */
static std::string	findAnyChannel(std::string msg_to_parse){
	std::string	channel;
	channel.clear();

	if (msg_to_parse.empty() || msg_to_parse.find("#") == msg_to_parse.npos)
		return (channel);
	else{
		size_t i = 0;
		while (msg_to_parse[i] && (!isalpha(msg_to_parse[i]) 
			&& !isdigit(msg_to_parse[i]) && msg_to_parse[i] != '-' && msg_to_parse[i] != '_'))
			i++;
		while (msg_to_parse[i] && (isalpha(msg_to_parse[i]) || msg_to_parse[i] == '-' 
			|| msg_to_parse[i] == '_' || isdigit(msg_to_parse[i])))
			channel += msg_to_parse[i++];
		return (channel);
	}
}

/**
 * @brief Generates a response string for the LIST command.
 * 
 * This function generates a response string for the LIST command, providing information
 * about a specific channel.
 * 
 * @param client_nick The nickname of the client making the request.
 * @param channel An iterator pointing to the channel for which the response is generated.
 * @return A string containing the response for the LIST command.
 */
static std::string	getRplList(std::string client_nick, std::map<std::string, Channel>::iterator &channel){
	std::stringstream concat;
	
	if (channel->second.getMode().find("p") != std::string::npos
		&& channel->second.doesClientExist(client_nick) == false){
		concat << client_nick << " #" << channel->second.getName() << " " \
		<< channel->second.getClientList().size() << " " \
		<< ":The topic of this channel is private." \
		<< "\r\n";
	} 
	else{
		concat << client_nick << " #" << channel->second.getName() << " "  \
		<< channel->second.getClientList().size() << " "\
		<< (channel->second.getTopic().empty() ? ":No topic set for this channel yet."  : channel->second.getTopic()) \
		<< "\r\n";
	}
	return (concat.str());			
}

/**
 * @brief Processes the LIST command and generates a response containing information about channels.
 * 
 * This function processes the LIST command from a client and generates a response containing
 * information about channels on the server.
 * 
 * @param server A pointer to the Server object.
 * @param client_fd The file descriptor of the client.
 * @param cmd_infos The parsed command information.
 */
void	list(Server *server, int const client_fd, s_cmd cmd_infos)
{
	std::string channel_to_display	= findAnyChannel(cmd_infos.message);
	Client&		client 				= retrieveClient(server, client_fd);
	std::string client_nick 		= client.getNickname();
	std::string	RPL_LISTSTART		= client_nick + " Channel :Users Name\r\n";
	std::string	RPL_LIST;
	std::string	RPL_LISTEND 		= client_nick + " :End of /LIST\r\n";

	if (channel_to_display.empty()){
		if (server->getChannels().empty()){
			addToClientBuffer(server, client_fd, RPL_LISTEND);
		} 
		else{
			addToClientBuffer(server, client_fd, RPL_LISTSTART);
			std::map<std::string, Channel>::iterator it = server->getChannels().begin();
			while (it != server->getChannels().end()) {
				RPL_LIST.clear();
				if (it->second.getMode().find('s') == std::string::npos
					|| (it->second.getMode().find('s') != std::string::npos 
						&& it->second.doesClientExist(client.getNickname()) == true)){
					RPL_LIST = getRplList(client_nick, it);
					addToClientBuffer(server, client_fd, RPL_LIST);
				}
				it++;
			}
			addToClientBuffer(server, client_fd, RPL_LISTEND);
		}
	}
	else{
		std::map<std::string, Channel>			 channels = server->getChannels();
		std::map<std::string, Channel>::iterator channel = channels.find(channel_to_display);
		if (channel == channels.end() \
			|| (channel->second.getMode().find('s') != std::string::npos \
				&& channel->second.doesClientExist(client.getNickname()) == false)){	
			addToClientBuffer(server, client_fd, RPL_LISTEND);
		} 
		else{
			RPL_LIST = getRplList(client_nick, channel);
			addToClientBuffer(server, client_fd, RPL_LIST);
		}
	}
	return ;
}
