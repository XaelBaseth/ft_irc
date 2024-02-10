/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axel <axel@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 12:22:00 by axel              #+#    #+#             */
/*   Updated: 2024/02/10 12:22:01 by axel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

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
class Server;
class Channel
{
	private:
		std::map<std::string, Client>	_clientList;
		std::vector<std::string>		_kicked_users;
		std::vector<std::string>		_operators;
		std::vector<std::string>		_banned_users;
		std::vector<std::string>		_voiced_users;
		std::string						_name;
		std::string						_operatorPassword;
		std::string						_topic;
		std::string						_mode;
		std::string						_channel_password;
		int								_capacity_limit;
	public:
		Channel(std::string const &Name);
		~Channel();
		
		std::string						&getName();
		std::string						&getTopic();
		std::string						&getMode();
		std::string						&getChannelPassword();
		int								&getCapacityLimit();
		std::map<std::string, Client>	&getClientList();
		std::vector<std::string>		&getBannedUsers();
		std::vector<std::string>		&getKickedUsers();
		std::vector<std::string>		&getOperators();
		void							setTopic(std::string &newTopic);
		void							setChannelPassword(std::string password);
		void							setCapacityLimit(int limit);

		bool							doesClientExist(std::string &clientName);
		void							addClientToChannel(Client &client);
};


/****************************************************************
* 							FUNCTIONS							*
*****************************************************************/


#endif