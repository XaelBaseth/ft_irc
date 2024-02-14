/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorMessage.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axel <axel@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 12:22:10 by axel              #+#    #+#             */
/*   Updated: 2024/02/14 11:47:56 by axel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORMESSAGE_HPP
# define ERRORMESSAGE_HPP

# define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")

# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
# define RPL_YOURHOST(client, servername, version) (":localhost 002 " + client + " :Your host is " + servername + " (localhost), running version " + version + "\r\n")
# define RPL_CREATED(client, datetime) (":localhost 003 " + client + " :This server was created " + datetime + "\r\n")
# define RPL_MYINFO(client, servername, version, user_modes, chan_modes, chan_param_modes) (":localhost 004 " + client + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
# define RPL_ISUPPORT(client, tokens) (":localhost 005 " + client + " " + tokens " :are supported by this server\r\n")

# define ERR_SERV_FULL "[Server] You cannot join, the server is already full"
# define ERR_UNKNOWNCOMMAND(client, command) ("[Client] :localhost 421 " + client + " " + command + " :Unknown command\r\n")

//MOTD
#define ERR_NOMOTD(client) (":localhost 422 " + client + " :MOTD File is missing\r\n")
#define RPL_MOTDSTART(client, servername) (":localhost 375 " + client + " :- " + servername + " Message of the day - \r\n")
#define RPL_MOTD(client, motd_line) (":localhost 372 " + client + " :" + motd_line + "\r\n")
#define RPL_ENDOFMOTD(client) (":localhost 376 " + client + " :End of /MOTD command.\r\n")


#endif