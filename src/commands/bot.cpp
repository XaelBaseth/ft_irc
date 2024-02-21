/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acharlot <acharlot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:36:49 by acharlot          #+#    #+#             */
/*   Updated: 2024/02/21 13:59:40 by acharlot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/ircserv.hpp"

/**
 * *Prototype for the command => `/msg raultbot: command`
*/

static void	botRandom(Server *server, int const client_fd, std::map<const int, Client>::iterator it_client, std::string bot)
{
	srand(time(NULL));
	int index = rand() % 10 + 1;
	
	std::string str;
	switch (index)
	{
		case 1: str = "Wearing headphones for just an hour could increase the bacteria in your ear by 700 times."; break;
		case 2: str = "Google images was literally created after Jennifer Lopez wore that infamous dress at the 2000 Grammys"; break;
		case 3: str = "Los Angeles' full name is 'El Pueblo de Nuestra Senora la Reina de los Angeles de Porciuncula'"; break;
		case 4: str = "Tigers have striped skin, not just striped fur."; break;
		case 5: str = "Like fingerprints, everyone's tongue print is different."; break;
		case 6: str = "Cat urine glows under a black-light."; break;
		case 7: str = "A shrimp's heart is in its head."; break;
		case 8: str = "The Spice Girls were originally a band called Touch."; break;
		case 9: str = "The unicorn is the national animal of Scotland"; break;
		case 10: str = "In 2014, there was a Tinder match in Antarctica"; break;
	}
	addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, it_client->second.getNickname(), str));
}

void	bot(Server *server, int const client_fd, std::map<const int, Client>::iterator it_client, std::string bot_cmd){
	std::string bot = "raultbot";
	std::string	validCmds[4] = {
		":HELP",
		":RANDOM",
		};

	for (size_t i = 0; i < bot_cmd.size(); i++)
		bot_cmd[i] = std::toupper(bot_cmd[i]);
	
	int index = 0;
	while (index < 4)
	{
		if (bot_cmd == validCmds[index])
			break;
		index++;
	}
	switch (index + 1)
	{
		case 1: addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, it_client->second.getNickname(), "Ask me 'RANDOM'")); break;
		case 2: botRandom(server, client_fd, it_client, bot); break;
		default:
			addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, it_client->second.getNickname(), "Invalid request, ask 'HELP' for more infos"));
	}
}