#ifndef SERVER_HPP
# define SERVER_HPP

/****************************************************************
* 							LIBRAIRIES							*
*****************************************************************/

# include "ircserv.hpp"

/****************************************************************
* 							MACRO								*
*****************************************************************/

# define FAILURE -1
# define SUCCESS 0
# define BREAK 2
# define CONTINUE 3
# define BACKLOG 10
# define MAX_CLIENT 4

extern bool	server_shutdown;
/****************************************************************
* 							CLASSES								*
*****************************************************************/

class Server {
private:
	std::string		_port;
	std::string		_password;
	struct addrinfo	_hints;
	struct addrinfo *_servinfo;
	int				_server_socket_fd;
public:
	Server();
	~Server(void);
	//Server.cpp
	void	setHints(void);
	int		launchServer(void);
	int		fillInfos(char *port);
	void 	addClient(int client_socket, std::vector<pollfd> &poll_fds);
	//manageServerLoop
	int		manageServerLoop(void);
	//manageServerUtils
	int		createClientConnexion(std::vector<pollfd>& poll_fds, std::vector<pollfd>& new_pollfds);
	int 	handleExistingConnexion(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it);
	int 	handlePolloutEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it, const int current_fd);
	int 	handlePollerEvent(std::vector<pollfd>& poll_fds, std::vector<pollfd>::iterator &it);

	class InvalidClientException : public std::exception{
		public:
				const char *what(void) const throw();
	};
	
};

/****************************************************************
* 							FUNCTIONS							*
*****************************************************************/


#endif