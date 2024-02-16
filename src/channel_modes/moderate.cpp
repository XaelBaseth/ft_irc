#include "../../lib/ircserv.hpp"


/**
 * @brief Modify the mode of a channel to enable or disable moderation.
 * 
 * This function modifies the mode of a channel to enable or disable moderation.
 * 
 * @param server Pointer to the Server instance.
 * @param mode_infos Structure containing mode information.
 * @param client_fd File descriptor of the client making the request.
 * @param mode_str The mode string.
 */
void	moderateChannelMode(Server *server, s_mode mode_infos, int const client_fd, std::string mode_str){
	std::map<const int, Client>::iterator	it_client = server->getClients().find(client_fd);
	(void)it_client; //used for the parameters of the prototype
	std::map<std::string, Channel>::iterator	it_channel_target = server->getChannels().find(mode_infos.target);

	size_t	pos = it_channel_target->second.getMode().find("m");
	if (mode_str[0] == '+'){
		if (pos != std::string::npos)
			return ;
		it_channel_target->second.addMode("m");
		broadcastToAllChannelMembers(server, it_channel_target->second, MODE_CHANNELMSG(mode_infos.target, "+m"));
	}
	else{
		if (pos == std::string::npos)
			return ;
		it_channel_target->second.removeMode("m");
		broadcastToAllChannelMembers(server, it_channel_target->second, MODE_CHANNELMSG(mode_infos.target, "-m"));
	}
}