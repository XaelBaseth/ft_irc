/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axel <axel@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 12:22:13 by axel              #+#    #+#             */
/*   Updated: 2024/02/14 14:15:08 by axel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include "Command.hpp"

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

Client*		getClient(Server *server, int const client_fd);
void		sendServerRpl(int const client_fd, std::string client_buffer);
void		sendClientRegistration(Server *server, int const client_fd, std::map<const int, Client>::iterator &it);
void		addToClientBuffer(Server *server, int const client_fd, std::string reply);
Client		&retrieveClient(Server *server, int const client_fd);
void		sendClientRegistration(Server *server, int const client_fd, std::map<const int, Client>::iterator &it);
std::string	getSymbol(Channel &channel);
std::string	getListOfMembers(std::string client, Channel &channel);
std::string	getChannelName(std::string msg_to_parse);
	
#endif