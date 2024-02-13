#ifndef IRCSERV_HPP
# define IRCSERV_HPP

/****************************************************************
* 							LIBRAIRIES							*
*****************************************************************/

# include <iostream>
# include <exception>
# include <netdb.h>
# include <csignal>
# include <cerrno>
# include <cstring>
# include <poll.h>
# include <vector>
# include <map>
# include <unistd.h>
# include <fstream>

# include "Colors.hpp"
# include "Server.hpp"
# include "ErrorMessage.hpp"
# include "Client.hpp"
# include "Channel.hpp"

/****************************************************************
* 							LIBRAIRIES							*
*****************************************************************/
# define FAILURE -1
# define SUCCESS 0
# define BREAK 2
# define CONTINUE 3
# define BACKLOG 10
# define MAX_CLIENT 4
# define MSG_SIZE 4096

extern bool	server_shutdown;
/****************************************************************
* 							CLASSES								*
*****************************************************************/

class Server;

/****************************************************************
* 							FUNCTIONS							*
*****************************************************************/

Client*	getClient(Server *server, int const client_fd);
void	sendServerRpl(int const client_fd, std::string client_buffer);

#endif