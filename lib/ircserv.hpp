/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acharlot <acharlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 12:22:13 by axel              #+#    #+#             */
/*   Updated: 2024/02/20 15:17:46 by acharlot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

/****************************************************************
* 							LIBRAIRIES							*
*****************************************************************/

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <cstring>
# include <cstdlib>
# include <iostream>
# include <vector>
# include <map>
# include <poll.h>
# include <unistd.h>
# include <sstream>
# include <fstream>
# include <csignal>
# include <string>
# include <cerrno>
# include <ctime>

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
std::string	getReason(std::string msg_to_parse);
void	broadcastToAllChannelMembers(Server *server, Channel &channel, std::string reply);

//MODE
void	banChannelMode(Server *server, std::string datas[4], int const client_fd);
void	keyChannelMode(Server *server, s_mode mode_infos, int const client_fd, std::string mode_str);
void	limitChannelMode(Server *server, std::string datas[4], int const client_fd);
void	moderateChannelMode(Server *server, s_mode mode_infos, int const client_fd, std::string mode_str);
void	operatorChannelMode(Server *server, s_mode mode_infos, int const client_fd, std::string str);
void	privateChannelMode(Server *server, s_mode mode_infos, int const client_fd, std::string mode_str);
void	secretChannelMode(Server *server, s_mode mode_infos, int const client_fd, std::string mode_str);
void	topicChannelMode(Server *server, s_mode mode_infos, int const client_fd, std::string mode_str);
void	voiceChannelMode(Server *server, s_mode mode_infos, int const client_fd, std::string str);

#endif