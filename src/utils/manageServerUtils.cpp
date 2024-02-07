#include "../../lib/Server.hpp"

static int	acceptSocket(int listenSocket) {
	sockaddr_in	client;
	socklen_t	addr_size = sizeof(sockaddr_in);
	return (accept(listenSocket, (sockaddr *)&client, &addr_size));
}

static void	tooManyClients(int client_socket) {
	std::cout << ToColor(ERR_SERV_FULL, Colors::Red) << std::endl;
	send(client_socket, ERR_SERV_FULL, strlen(ERR_SERV_FULL) + 1, 0);
	close(client_socket);
}

int	Server::createClientConnexion(std::vector<pollfd>& poll_fds, std::vector<pollfd>& new_pollfds) {
	int	client_socket = acceptSocket(_server_socket_fd);
	if (client_socket == -1) {
		std::cerr << ToColor("[Error] unable to accept", Colors::Red) << std::endl;
		return (CONTINUE);
	}
	if (poll_fds.size() - 1 < MAX_CLIENT)
		addClient(client_socket, new_pollfds);
	else
		tooManyClients(client_socket);
	return (SUCCESS);
}

int Server::handleExistingConnexion(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it) {
	std::cerr << ToColor("TODO", Colors::DeepPink) << std::endl;
	//TODO: handle client
}

int Server::handlePolloutEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it, const int current_fd) {
	std::cerr << ToColor("TODO", Colors::DeepPink) << std::endl;
	//TODO: handle client disconnection
}

int Server::handlePollerEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it) {
	if (it->fd == _server_socket_fd) {
		std::cerr << ToColor("[Error] listen socket error", Colors::Red) << std::endl;
		return (FAILURE);
	}
	else
		return (BREAK);
}