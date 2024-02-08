#include "../../lib/ircserv.hpp"

bool	server_shutdown;

int	Server::manageServerLoop(){
	std::vector<pollfd> 	poll_fds;
	pollfd					server_poll_fd;

	server_poll_fd.fd = _server_socket_fd;
	server_poll_fd.events = POLLIN;

	poll_fds.push_back(server_poll_fd);

	while (server_shutdown == false) {
		std::vector<pollfd> new_pollfds; //!tmp to host the newly-created fds

		if (poll((pollfd *)&poll_fds[0], (unsigned int)poll_fds.size(), -1) <= SUCCESS) {
			if (errno == EINTR)
				break ;
			std::cerr << ToColor("[Error] poll error", Colors::Red) << std::endl;
			throw ;
		}

		std::vector<pollfd>::iterator it = poll_fds.begin();
		while (it != poll_fds.end()) {
			if (it->revents & POLLIN) { //data is ready to be received()
				if (it->fd == _server_socket_fd) {
					if (this->createClientConnexion(poll_fds, new_pollfds) == CONTINUE)
						continue;
				}
				else { //dedicated fd for Client/Server connection exists
					if (this->handleExistingConnexion(poll_fds, it) == BREAK)
						break ;
				}
			}
			else if (it->revents & POLLOUT) { //alert when data can be sent without blocking
				if (handlePolloutEvent(poll_fds, it, it->fd) == BREAK)
					break ;
			}
			else if (it->revents & POLLERR) {
				if (handlePollerEvent(poll_fds, it) == BREAK)
					break ;
				else
					return (FAILURE);
			}
			it += 1;
		}
		poll_fds.insert(poll_fds.end(), new_pollfds.begin(), new_pollfds.end()); // Add the range of NEW_pollfds in poll_fds
	}
	return (SUCCESS);
}