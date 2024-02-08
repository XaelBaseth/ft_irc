#ifndef SERVER_HPP
# define SERVER_HPP

/****************************************************************
* 							LIBRAIRIES							*
*****************************************************************/

# include "ircserv.hpp"

/****************************************************************
* 							MACRO								*
*****************************************************************/


/****************************************************************
* 							CLASSES								*
*****************************************************************/

class Client;
class Server {
	private:
		std::string		_port;
		std::string		_password;
		struct addrinfo	_hints;
		struct addrinfo *_servinfo;
		int				_server_socket_fd;
		std::map<const int, Client>	_clients;
	public:
		Server();
		~Server(void);
		//Server.cpp
		std::map<const int, Client>&	getClients();
		void	setHints(void);
		int		launchServer(void);
		int		fillInfos(char *port);
		void 	addClient(int client_socket, std::vector<pollfd> &poll_fds);
		void	delClient(std::vector<pollfd> &poll_fds, std::vector<pollfd>::iterator &it, int current_fd);
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