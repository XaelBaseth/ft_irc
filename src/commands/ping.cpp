#include "../../lib/ircserv.hpp"

int	ping(Server *server, int const client_fd, s_cmd &cmd){
	Client		&client		= retrieveClient(server, client_fd);
	std::string	nickname	= client.getNickname();
	std::string	username	= client.getUsername();

	if (cmd.message[0] == ' ')
		cmd.message.erase(0, 1);
	cmd.message.insert(0, ":");
	addToClientBuffer(server, client_fd, RPL_PONG(user_id(nickname, username), cmd.message));

	return (SUCCESS);
}