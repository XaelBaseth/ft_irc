#include "../../lib/ircserv.hpp"

/**
 * @brief Modify the mode of a channel to voice or unvoice a user.
 * 
 * This function modifies the mode of a channel to voice or unvoice a user.
 * 
 * @param server Pointer to the Server instance.
 * @param mode_infos Structure containing mode information.
 * @param client_fd File descriptor of the client making the request.
 * @param str The mode string.
 */
void	voiceChannelMode(Server *server, s_mode mode_infos, int const client_fd, std::string str){
	std::map<const int, Client>::iterator it_client = server->getClients().find(client_fd);
	std::map<std::string, Channel>::iterator it_channel_target = server->getChannels().find(mode_infos.target);
	
	if (mode_infos.params.empty() == true)
		return ;

	std::map<const int, Client>::iterator it_user_target = server->getClients().begin();
    while (it_user_target != server->getClients().end()){
    	if (it_user_target->second.getNickname() == mode_infos.params)
        	break;
    	it_user_target++;
    }
	if (it_user_target == server->getClients().end()){
		addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(it_client->second.getNickname(), mode_infos.params));
		addToClientBuffer(server, client_fd, ERR_USERNOTINCHANNEL(it_client->second.getNickname(), mode_infos.params, mode_infos.target));
		return ;
	}
	if (it_channel_target->second.getClientList().find(mode_infos.params) == it_channel_target->second.getClientList().end()){
		addToClientBuffer(server, client_fd, ERR_USERNOTINCHANNEL(it_client->second.getNickname(), mode_infos.params, mode_infos.target));
		return ;
	}

	std::vector<std::string>::iterator it;
	for (it = it_channel_target->second.getVoicedUsers().begin(); it != it_channel_target->second.getVoicedUsers().end(); it++){
		if (*it == mode_infos.params)
			break;
	}
	if (str[0] == '+'){
		if (it != it_channel_target->second.getVoicedUsers().end())
			return ;
		
		it_channel_target->second.getVoicedUsers().push_back(mode_infos.params);
		broadcastToAllChannelMembers(server, it_channel_target->second, RPL_ADDVOICE(it_client->second.getNickname(), it_client->second.getUsername(), mode_infos.target, "+v", mode_infos.params));
	}
	else{
		if (it == it_channel_target->second.getVoicedUsers().end())
			return ;
		it_channel_target->second.getVoicedUsers().erase(it);
		broadcastToAllChannelMembers(server, it_channel_target->second, RPL_ADDVOICE(it_client->second.getNickname(), it_client->second.getUsername(), mode_infos.target, "-v", mode_infos.params));
	}
}