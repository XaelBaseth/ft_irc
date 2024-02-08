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

static void	print(std::string type, int client_socket, char *message) {
	if (message)
		std::cout << std::endl << type << client_socket << ": " << ToColor(message, Colors::RoyalBlue);
}

int Server::handleExistingConnexion(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it) {
	Client	*client;
	client = getClient(this, it->fd);
	char	message[MSG_SIZE];
	int		read_count;

	memset(message, 0, sizeof(message));
	read_count = recv(it->fd, message, MSG_SIZE, 0);

	if (read_count <= FAILURE) {
		std::cerr << ToColor("[Server] recv() failed", Colors::Red) << std::endl;
		delClient(poll_fds, it, it->fd);
		return (BREAK);
	}
	else if (read_count == 0) {
		std::cout << ToColor("[Server] A client just disconnected", Colors::Orange) << std::endl;
		delClient(poll_fds, it, it->fd);
		return (BREAK);
	}
	else {
		print("[Client] Message received from client #", it->fd, message); //!if problem check HERE
		client->setReadBuffer(message);

		if (client->getReadBuffer().find("\r\n") != std::string::npos) {
			try {
				//parseMessage(it->fd, client->getReadBuffer());
				if (client->getReadBuffer().find("\r\n"))
					client->getReadBuffer().clear();
			}
			catch(const std::exception& e) {
				std::cout << ToColor("[Server] caught exception: ", Colors::Red);
				std::cerr << ToColor(e.what(), Colors::Red) << std::endl;
				//TODO:Uncomment when client IRC is done
				//if (client->isRegistrationDone() == true)
				//	client->setDeconnexionStatus(true);
				return (BREAK);
			}
		}
	}
	return (SUCCESS);
}

int Server::handlePolloutEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it, const int current_fd) {
	Client	*client = getClient(this, current_fd);
	if (!client)
		std::cout << ToColor("[Server] Could not manage to find a connexion to client.", Colors::Red) << std::endl;
	else {
		sendServerRpl(current_fd, client->getSendBuffer());
		client->getSendBuffer().clear();
		//TODO: Uncomment when client IRC is done
		(void)it;
		(void)poll_fds;
		//if (client->getDeconnexionStatus() == true) {
		//	delClient(poll_fds, it, current_fd);
		//	return (BREAK);
		//}
	}
	return (SUCCESS);
}

int Server::handlePollerEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it) {
	if (it->fd == _server_socket_fd) {
		std::cerr << ToColor("[Error] listen socket error", Colors::Red) << std::endl;
		return (FAILURE);
	}
	else {
		delClient(poll_fds, it, it->fd);
		return (BREAK);
	}
}