#include "../../lib/ircserv.hpp"

/**
 * *Need to be an operator to change it.
 * *Prototype for the command => `/topic #channel :Topic`
 * 
 * *For users to view
 * *Prototype for the command => `/topic`
*/

std::string	findChannelName(std::string msg_to_parse){
	std::string channel_name;
	channel_name.clear();
	
	if (msg_to_parse.empty() || msg_to_parse.find("#") == msg_to_parse.npos)
		return (channel_name);
	else if (msg_to_parse.find(":") != msg_to_parse.npos){
		char *str = const_cast<char *>(msg_to_parse.data());
		channel_name = strtok(str, " ");
		channel_name.erase(channel_name.find("#"), 1);
	}
	else{
		size_t i = 0;
		while (msg_to_parse[i] && (!isalpha(msg_to_parse[i]) && !isdigit(msg_to_parse[i]) && msg_to_parse[i] != '-' && msg_to_parse[i] != '_'))
			i++;
		while (msg_to_parse[i] && (isalpha(msg_to_parse[i]) || msg_to_parse[i] == '-' || msg_to_parse[i] == '_' || isdigit(msg_to_parse[i])))
			channel_name += msg_to_parse[i++];
	}
	return (channel_name);
}

std::string	findTopic(std::string msg_to_parse){
	std::string topic;

	if (msg_to_parse.empty() || msg_to_parse.find(":") == msg_to_parse.npos)
		topic.clear();
	else
		topic.append(msg_to_parse, msg_to_parse.find(":"), std::string::npos);
	return (topic);
}


static void	broadcastToChannel(Server *server, Channel &channel, Client &client,
 std::string channel_name, std::string topic){
	std::map<std::string, Client>::iterator member = channel.getClientList().begin();
	std::string								client_nickname = client.getNickname();
	
	while (member != channel.getClientList().end()){
		addToClientBuffer(server, member->second.getClientFd(), RPL_TOPIC(client_nickname, channel_name, topic));
		member++;
	}
}

void	topic(Server *server, int const client_fd, s_cmd cmd_infos){
	std::string channel_name;
	std::string	topic;
	
	Client&		client = retrieveClient(server, client_fd);
	std::string client_nickname = client.getNickname();

	channel_name = findChannelName(cmd_infos.message);
	if (channel_name.empty()){
		addToClientBuffer(server, client_fd, ERR_NEEDMOREPARAMS(client_nickname, cmd_infos.name));
		return ;
	}

	std::map<std::string, Channel>&			 channels = server->getChannels();
	std::map<std::string, Channel>::iterator channel = channels.find(channel_name);
	if (channel == channels.end()){
		addToClientBuffer(server, client_fd, ERR_NOSUCHCHANNEL(client_nickname, channel_name));
		return ;
	}
	if (channel->second.doesClientExist(client_nickname) == false){
		addToClientBuffer(server, client_fd, ERR_NOTONCHANNEL(client_nickname, channel_name));
		return ;
	}

	topic = findTopic(cmd_infos.message);
	
	if (topic.empty()){
		if (channel->second.getTopic().empty() == false)
			addToClientBuffer(server, client_fd, RPL_TOPIC(client_nickname, channel_name, channel->second.getTopic()));
		else
			addToClientBuffer(server, client_fd, RPL_NOTOPIC(client_nickname, channel_name));
	}
	else{
		if (channel->second.getMode().find('t') != std::string::npos
			&& channel->second.isOperator(client_nickname) == false)
			addToClientBuffer(server, client_fd, ERR_CHANOPRIVSNEEDED(client_nickname, channel_name));
		else{
			if (topic == ":")
				topic.clear();
			channel->second.setTopic(topic);
			broadcastToChannel(server, channel->second, client, channel_name, topic);
		}
		
	}

}