#include "../../lib/ircserv.hpp"

/**
 * *Prototype for the command 	=> `/part #channel :Reason`
*/

static bool			containsAtLeastOneAlphaChar(std::string str)
{
	for (size_t i = 0; i < str.size(); i++){
		if (isalpha(str[i]))
			return (true);
	}
	return (false);
}

void				part(Server *server, int const client_fd, s_cmd cmd_infos){
	Client		client		= retrieveClient(server, client_fd);
	std::string nick		= client.getNickname();
	std::string	channel;

	std::string reason = getReason(cmd_infos.message);
	if (cmd_infos.message.empty() == true){
		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(nick, cmd_infos.name));
		return ;
	}
	cmd_infos.message.erase(cmd_infos.message.find(reason), reason.length());

	while (containsAtLeastOneAlphaChar(cmd_infos.message) == true){
		channel.clear();
		channel = getChannelName(cmd_infos.message);
		cmd_infos.message.erase(cmd_infos.message.find(channel), channel.length()); 

		std::map<std::string, Channel>&			 channels 	= server->getChannels();
		std::map<std::string, Channel>::iterator it			= channels.find(channel);
		if (it == channels.end())
		{
			addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(nick, channel));
			continue ;
		}
		else if (it != channels.end() \
				&& it->second.doesClientExist(nick) == false){
			addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(nick, channel));
			continue ;
		}
		else{
			it->second.getClientList().erase(nick);
			addToClientBuffer(server, client_fd, RPL_PART(user_id(nick, client.getUsername()), channel, reason));
			broadcastToAllChannelMembers(server, it->second, client.getUsername());
		}
	}
}