#include "../../lib/ircserv.hpp"

Client*	getClient(Server *server, int const client_fd)
{
	std::map<const int, Client>&		client_list = server->getClients();
	std::map<const int, Client>::iterator it_client = client_list.find(client_fd);
	
	if (it_client == client_list.end())
		return (NULL);
	return (&it_client->second);
}

void	sendServerRpl(int const client_fd, std::string client_buffer) {
	std::istringstream	buf(client_buffer);
	std::string			reply;

	send(client_fd, client_buffer.c_str(), client_buffer.size(), 0);
	while (getline(buf, reply)) {
		std::cout << ToColor("[Server] Message sent to client #", Colors::Blue)
			<< client_fd << ToColor(reply, Colors::Blue) << std::endl;
	}
}
