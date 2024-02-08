#ifndef CLIENT_HPP
# define CLIENT_HPP

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

class Client {
	private:
		int			_client_fd;
		std::string	_sendbuf;
		std::string	_readbuf;
	public:
		Client(int client_fd);
		~Client(void);
		int				getClientFd()const;
		std::string&	getSendBuffer();
		std::string&	getReadBuffer();
		void			setReadBuffer(std::string const &buf);
};

/****************************************************************
* 							FUNCTIONS							*
*****************************************************************/

#endif