#include "../../lib/ircserv.hpp"

void	inviteOnlyMode(Server *server, std::string datas[4], int const client_fd){
	if (server->isChannel(datas[1]) == false){
		addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(datas[0], datas[1]));
		return ;
	}

	std::map<std::string, Channel>::iterator	it;
	it = server->getChannels().find(datas[1]);
	std::list<std::string>						invitedUsers = it->second.getListInvited();

	if (datas[2][0] == '+'){
		it->second.setInviteOnly(invitedUsers);
		size_t	pos = it->second.getMode().find("i");
		if (pos != std::string::npos)
			return ;
		it->second.addMode("i");
		broadcastToAllChannelMembers(server, it->second, MODE_CHANNELMSG(datas[1], "+i"));
	}
	else if (datas[2][0] == '-'){
		invitedUsers.clear();
		size_t pos = it->second.getMode().find("i");
		if (pos == std::string::npos)
			return ;
		it->second.removeMode("i");
		broadcastToAllChannelMembers(server, it->second, MODE_CHANNELMSG(datas[1], "-i"));
	}
}