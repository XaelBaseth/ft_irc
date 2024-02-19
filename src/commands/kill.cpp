#include "../../lib/ircserv.hpp"

/**
 * *Need to be an operators.
 * *Prototype for the command => `/kick #channel nickname :Reason`
*/

/**
 * @brief Removes a client from all channels in the server.
 * 
 * This function removes a client from all channels in the server by iterating through
 * the list of channels and removing the client from each channel's client list if present.
 * 
 * @param server A pointer to the Server object.
 * @param killed_user The nickname of the client to be removed from the server.
 */
static void	removeFromServer(Server *server, std::string killed_user){
	std::map<std::string, Channel>&				channels = server->getChannels();
	std::map<std::string, Channel>::iterator	chan;

	for (chan = channels.begin(); chan != channels.end(); chan++){
		if (chan->second.doesClientExist(killed_user))
			chan->second.getClientList().erase(killed_user);
	}
}

/**
 * @brief Broadcasts a quit message to all clients in the server.
 * 
 * This function sends a quit message to all clients in the server by iterating through
 * the list of clients and adding the quit message to their send buffers.
 * 
 * @param server A pointer to the Server object.
 * @param reply The quit message to broadcast.
 */
static void	broadcastQuit(Server *server, std::string reply){
    std::map<const int, Client>&		client_list = server->getClients();
	std::map<const int, Client>::iterator it_client;
	
	for (it_client = client_list.begin(); it_client != client_list.end(); it_client++)
		addToClientBuffer(server, it_client->second.getClientFd(), reply);
}

/**
 * @brief Checks if the given nickname belongs to an IRC operator.
 * 
 * This function checks if the specified nickname belongs to an IRC operator by searching
 * in the list of IRC operators in the server.
 * 
 * @param server A pointer to the Server object.
 * @param nickname The nickname to check.
 * @return true if the nickname belongs to an IRC operator, false otherwise.
 */
static bool	isIrcOperator(Server *server, std::string nickname){
	std::vector<server_op> irc_ops = server->getIRCOperators();
	std::vector<server_op>::iterator it_op;
	
	for (it_op = irc_ops.begin(); it_op != irc_ops.end(); it_op++)
	{
		if (it_op->name == nickname)
			return (true);
	}
	return (false);
}

/**
 * @brief Checks if a client with the given nickname exists in the server.
 * 
 * This function checks if a client with the specified nickname exists in the server's client list.
 * 
 * @param server A pointer to the Server object.
 * @param nickname The nickname to search for.
 * @return A pointer to the Client object if found, otherwise NULL.
 */
static Client*	clientExists(Server *server, std::string nickname){
	std::map<const int, Client>&		client_list = server->getClients();
	std::map<const int, Client>::iterator it_client;
	
	for (it_client = client_list.begin(); it_client != client_list.end(); it_client++){
		if (it_client->second.getNickname() == nickname)
			break;
	}
	if (it_client == client_list.end())
		return (NULL);
	return (&it_client->second);
}

/**
 * @brief Retrieves the nickname from a message string.
 * 
 * This function extracts the nickname from a message string, which typically follows the command.
 * 
 * @param msg The message string.
 * @return The extracted nickname.
 */
static std::string	retrieveNickname(std::string msg){
	std::string nickname;

	if (msg[0] == ' ')
		msg.erase(0,1);
	if (msg.empty()){
		nickname.clear();
		return (nickname);
	}
	else{
		if (msg.find(' ') != std::string::npos)
			nickname.insert(0, msg, 0, msg.find_first_of(' '));
		else
			nickname.insert(0, msg, 0, msg.npos);
	}
	return (nickname);
}

/**
 * @brief Retrieves the comment part of a message.
 * 
 * This function extracts the comment part from a message string, 
 * which typically follows the command and parameters.
 * 
 * @param msg The message string.
 * @return The comment part of the message.
 */
static std::string retrieveComment(std::string msg){
	std::string comment;

	if (msg[0] == ' ')
		msg.erase(0,1);
	if (msg.empty() || msg.find(' ') == std::string::npos){
		comment.clear();
		return (comment);
	}
	else{
		if (msg.find(' ') != std::string::npos)
			comment.insert(0, msg,  msg.find_first_of(' ') + 1, msg.npos);
	}
	return (comment);
}

/**
 * @brief Terminates a client's connection.
 * 
 * This function terminates the connection of a specified client, given by their nickname.
 * The client initiating the kill command must either have operator privileges or be an IRC operator.
 * 
 * @param server The server instance.
 * @param client_fd The file descriptor of the client initiating the kill command.
 * @param cmd_infos The parsed command information.
 */
void	kill(Server *server, int const client_fd, s_cmd cmd_infos){
	Client &client = retrieveClient(server, client_fd);
	std::string killer = client.getNickname();
	std::string killed = retrieveNickname(cmd_infos.message);
	std::string	comment = retrieveComment(cmd_infos.message);
	Client *killed_user = clientExists(server, killed);
	
	if (killed.empty())
		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(killer, cmd_infos.name));
	else if (killed_user == NULL)
		addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(killer, killed));
	else if (comment.empty())
		comment = "default";
	else if (client.getMode().find('o') == std::string::npos && isIrcOperator(server, killer) == false)
		addToClientBuffer(server, client_fd, ERR_NOPRIVILEGES(killer));
	else {
		addToClientBuffer(server, killed_user->getClientFd(), RPL_KILL(user_id(killer, client.getUsername()), killed, comment));

		std::string quit_reason = ":Killed (" + killer + " " + comment + ")";
		broadcastQuit(server, RPL_QUIT(user_id(killed, killed_user->getUsername()), quit_reason));
		
		std::string error_reason = ":Closing Link: localhost. Killed (" + killer + " " + comment + ")";
		addToClientBuffer(server, killed_user->getClientFd(), RPL_ERROR(user_id(killed, killed_user->getUsername()), error_reason));

		killed_user->setDeconnexionStatus(true);
		removeFromServer(server, killed);	
	}
}