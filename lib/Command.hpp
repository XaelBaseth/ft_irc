#ifndef COMMAND_HPP
# define COMMAND_HPP

/****************************************************************
* 							LIBRAIRIES							*
*****************************************************************/

# include "ircserv.hpp"

/****************************************************************
* 							LIBRAIRIES							*
*****************************************************************/
# define VALID_LEN 17


/****************************************************************
* 							CLASSES								*
*****************************************************************/

class Server;

struct	s_cmd
{
	std::string prefix;
	std::string name;
	std::string message;
};

struct s_mode
{
	std::string	target;
	std::string	mode;
	std::string	params;
};

/****************************************************************
* 							FUNCTIONS							*
*****************************************************************/

int		parseCommand(std::string cmd_line, s_cmd &cmd_infos);

void	invite(Server *server, int const client_fd, s_cmd cmd_infos);
void	join(Server *server, int const client_fd, s_cmd cmd_infos);
void	nick(Server *server, int const client_fd, s_cmd cmd_infos);
int		pass(Server *server, int const client_fd, s_cmd cmd_infos);

#endif