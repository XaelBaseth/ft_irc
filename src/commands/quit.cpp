#include "../../lib/ircserv.hpp"

/**
 * *Prototype for the command 	=> `/quit :Reason`
*/


static void	broadcastToChan(Server *server, Channel &channel, int const client_fd, 
std::string nick, std::string user, std::string reason){
	std::map<std::string, Client>::iterator member = channel.getClientList().begin();
	
	(void) client_fd;
	while (member != channel.getClientList().end()){
		if (member->second.getClientFd() != client_fd){
			addToClientBuffer(server, member->second.getClientFd(), \
				RPL_QUIT(user_id(nick, user), reason));
		}	
		member++;
	}
}

static void	removeFromServer(Server *server, std::string killed_user){
	std::map<std::string, Channel>&				channels = server->getChannels();
	std::map<std::string, Channel>::iterator	chan;
	for (chan = channels.begin(); chan != channels.end(); chan++){
		if (chan->second.doesClientExist(killed_user))
			chan->second.getClientList().erase(killed_user);
	}
}

void		quit(Server *server, int const client_fd, s_cmd cmd_infos){
	Client& 								  client   = retrieveClient(server, client_fd);
	std::string								  reason   = getReason(cmd_infos.message);
	std::map<std::string, Channel>&			  channels = server->getChannels();
	std::map<std::string, Channel>::iterator  chan	   = channels.begin();

	addToClientBuffer(server, client_fd, RPL_QUIT(user_id(client.getNickname(), client.getUsername()), reason));

	for (; chan != channels.end(); chan++){
		std::map<std::string, Client>& 			chan_members = chan->second.getClientList();
		std::map<std::string, Client>::iterator	member		 = chan_members.begin();
		for (; member != chan_members.end(); member++){
			if (member->second.getClientFd() == client_fd){
				chan_members.erase(client.getNickname());
				broadcastToChan(server, chan->second, client_fd, client.getNickname(), client.getUsername(), reason);
				break ;
			}
		}
	}
	client.setDeconnexionStatus(true);
	removeFromServer(server, client.getNickname());
}