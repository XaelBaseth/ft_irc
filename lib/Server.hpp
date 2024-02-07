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
# define BACKLOG 10

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
	void	setHints(void);
	int		launchServer(void);
	int		fillInfos(char *port);

	class InvalidClientException : public std::exception{
		public:
				const char *what(void) const throw();
	};
	
};

/****************************************************************
* 							FUNCTIONS							*
*****************************************************************/


#endif