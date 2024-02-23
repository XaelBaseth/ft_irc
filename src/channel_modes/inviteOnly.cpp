#include "../../lib/ircserv.hpp"


void	inviteOnlyMode(Server *server, std::string datas[4], int const client_fd){
	if (server->isChannel(datas[1]) == false){
		addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(datas[0], datas[1]));
		return ;
	}

	if (datas[3].empty() == true) {
	std::map<std::string, Channel>::iterator	it_channel_target = server->getChannels().find(datas[1]);
	size_t										pos = it_channel_target->second.getMode().find('i');

	if (datas[2][0] == '+'){
		if (pos != std::string::npos)
			return ;
		it_channel_target->second.addMode("i");
		broadcastToAllChannelMembers(server, it_channel_target->second, MODE_CHANNELMSG(datas[1], "+i"));
	}
	else{
			if (pos == std::string::npos)
				return;
			it_channel_target->second.removeMode("i");
			broadcastToAllChannelMembers(server, it_channel_target->second, MODE_CHANNELMSG(datas[1], "-i"));
		}
		return ;
	}
}