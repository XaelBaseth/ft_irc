#include "../../lib/ircserv.hpp"

/**
 * @brief Splits a message into individual commands.
 * 
 * This function splits the given message into individual commands based on the newline delimiter.
 * It populates the provided vector 'cmds' with the separated commands.
 * 
 * @param cmds A vector to store the individual commands.
 * @param msg The message to be split into commands.
 */
static void	splitMsg(std::vector<std::string> &cmds, std::string msg){
	int	pos = 0;
	std::string	delimiter = "\n";
	std::string	substr;

	while ((pos = msg.find(delimiter)) != static_cast<int>(std::string::npos)){
		substr = msg.substr(0, pos);
		cmds.push_back(substr);
		msg.erase(0, pos + delimiter.length());
	}
}

void Server::fillClients(std::map<const int, Client> &client_list, int client_fd, std::string cmd){
	std::map<const int, Client>::iterator it = client_list.find(client_fd);
	s_cmd cmd_infos;
	if (parseCommand(cmd, cmd_infos) == FAILURE)
		return ;

	if (cmd.find("NICK") != std::string::npos)
		nick(this, client_fd, cmd_infos);
	else if (cmd.find("USER") != std::string::npos)
		user(this, client_fd, cmd_infos);
	else if (cmd.find("PASS") != std::string::npos)
	{
		if (pass(this, client_fd, cmd_infos) == SUCCESS)
			it->second.setConnexionPassword(true);
		else
			it->second.setConnexionPassword(false);
	}
}

/**
 * @brief Executes a command received from a client.
 * 
 * This function parses the received command and executes the corresponding
 * action based on the command type. It checks for a valid command from the
 * list of supported commands and performs the associated action. If the
 * command is not recognized, it sends an error message to the client.
 * 
 * @param client_fd The file descriptor of the client.
 * @param cmd_line The command received from the client.
 */
void	Server::execCommand(int const client_fd, std::string cmd_line){
	std::string	validCmds[VALID_LEN] = {
		"INVITE", "JOIN", "KICK", "KILL", "LIST", "MODE",
		"MOTD", "NAMES", "NICK", "NOTICE", "OPER", "PART",
		"PING", "PRIVMSG", "QUIT", "TOPIC", "USER"
	};

	Client	*client = getClient(this, client_fd);
	s_cmd	cmd_infos;
	int	index = 0;

	if (parseCommand(cmd_line, cmd_infos) == FAILURE)
		return ;
	
	while (index < VALID_LEN){
		if (cmd_infos.name == validCmds[index])
			break ;
		index++;
	}

	switch (index + 1) {
		case 1: invite(this, client_fd, cmd_infos); break;
		case 2: join(this, client_fd, cmd_infos); break;
		case 3: kick(this, client_fd, cmd_infos); break;
		case 4: kill(this, client_fd, cmd_infos); break;
		case 5: list(this, client_fd, cmd_infos); break;
		case 6: modeFunction(this, client_fd, cmd_infos); break;
		case 7: motd(this, client_fd, cmd_infos); break;
		case 8: names(this, client_fd, cmd_infos); break;
		case 9: nick(this, client_fd, cmd_infos); break;
    	case 10: notice(this, client_fd, cmd_infos); break;
		case 11: oper(this, client_fd, cmd_infos); break;
		case 12: part(this, client_fd, cmd_infos); break;
		case 13: ping(this, client_fd, cmd_infos); break;
		case 14: privmsg(this, client_fd, cmd_infos); break;
		case 15: quit(this, client_fd, cmd_infos); break;
		case 16: topic(this, client_fd, cmd_infos); break;
		case 17: user(this, client_fd, cmd_infos); break;
		default:
			addToClientBuffer(this, client_fd, ERR_UNKNOWNCOMMAND(client->getNickname(), cmd_infos.name));
	}
}



/**
 * @brief Parses and processes a message received from a client.
 * 
 * This function splits the received message into individual commands and processes
 * each command based on the client's registration status. If the client is not fully
 * registered, it fills the client's information based on the commands. Once the client
 * is fully registered, it executes the commands using execCommand function.
 * 
 * @param client_fd The file descriptor of the client.
 * @param message The message received from the client.
 */
void	Server::parseMsg(int const client_fd, std::string message){
	std::vector<std::string>				cmds;
	std::map<const int, Client>::iterator	it = _clients.find(client_fd);

	splitMsg(cmds, message);

	for (size_t i = 0; i != cmds.size(); i++)
	{
		if (it->second.isRegistrationDone() == false)
		{
			if (it->second.hasAllInfo() == false)
			{
				fillClients(_clients, client_fd, cmds[i]);
				if (it->second.getNbInfo() == 3)
					it->second.hasAllInfo() = true;
			}
			if (it->second.hasAllInfo() == true && it->second.isWelcomeSent() == false)
			{
				if (it->second.isValid() == SUCCESS)
				{
					sendClientRegistration(this, client_fd, it);
					it->second.isWelcomeSent() = true;
					it->second.isRegistrationDone() = true;
				}		
				else
					throw Server::InvalidClientException();
			}
		}
		else
			execCommand(client_fd, cmds[i]);
	}
}



int	parseCommand(std::string cmd_line, s_cmd &cmd_infos)
{
	if (cmd_line.empty() == true)
		return (FAILURE);
	
	// COMMAND
	std::string copy = cmd_line;
	if (cmd_line[0] == ':')
	{
		if (cmd_line.find_first_of(' ') != std::string::npos)
			copy.erase(0, copy.find_first_of(' ') + 1);
	}
	
	if (copy.find_first_of(' ') == std::string::npos)
	{
		cmd_infos.name.insert(0, copy, 0, std::string::npos);
		if (cmd_infos.name.find('\r') != std::string::npos)
			cmd_infos.name.erase(cmd_infos.name.find('\r'), 1); 
	}
	else
		cmd_infos.name.insert(0, copy, 0, copy.find_first_of(' '));

	// PREFIX
	size_t prefix_length = cmd_line.find(cmd_infos.name, 0);
	cmd_infos.prefix.assign(cmd_line, 0, prefix_length);

	// MESSAGE
	size_t msg_beginning = cmd_line.find(cmd_infos.name, 0) + cmd_infos.name.length();
	cmd_infos.message = cmd_line.substr(msg_beginning, std::string::npos);
	cmd_infos.message.erase(cmd_infos.message.find("\r"), 1);

	for (size_t i = 0; i < cmd_infos.name.size(); i++)
		cmd_infos.name[i] = std::toupper(cmd_infos.name[i]);
	
	return (SUCCESS);
}
