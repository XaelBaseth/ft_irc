#ifndef SERVER_HPP
# define SERVER_HPP

/****************************************************************
* 							LIBRAIRIES							*
*****************************************************************/

# include "Colors.hpp"
# include "ircserv.hpp"

/****************************************************************
* 							CLASSES								*
*****************************************************************/

class Server {
private:
	std::string	_port;
	std::string	_password;
public:
	Server();
	~Server();
	class InvalidClientException : public std::exception{
		public:
				const char *what(void) const throw();
	};
};

/****************************************************************
* 							FUNCTIONS							*
*****************************************************************/


#endif