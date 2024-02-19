#include "../../lib/ircserv.hpp"

/**
 * *Prototype for the command 		=> `/privmsg nickname : 'msg' for an user.'
 * *								=> `/privmsg #channel : 'msg' for an user.'
*/

static void  broadcastToChannel(Server *server, int const client_fd, std::map<const int, Client>::iterator it_client,
std::map<std::string, Channel>::iterator it_channel, std::string target, std::string msg_to_send){
   std::vector<std::string> banned_users = it_channel->second.getBannedUsers();
   for (std::vector<std::string>::iterator it = banned_users.begin(); it != banned_users.end(); it++){
		if (*it == it_client->second.getNickname()){
			std::cout << ToColor("[Server] ", Colors::Red) << it_client->second.getNickname() 
			<< ToColor(" is banned from the channel and can't send messages anymore", Colors::Red) << std::endl;
			return ;
		}
	}
   std::vector<std::string> kicked_users = it_channel->second.getKickedUsers();
	for (std::vector<std::string>::iterator it = kicked_users.begin(); it != kicked_users.end(); it++){
		if (*it == it_client->second.getNickname())
		{
			std::cout << ToColor("[Server] ", Colors::Red) << it_client->second.getNickname() 
			<< ToColor(" is kicked from the channel and can't send messages anymore", Colors::Red) << std::endl;
			return ;
		}
	}
   if (it_channel->second.getMode().find("m") != std::string::npos){
      if (it_channel->second.isOperator(it_client->second.getNickname()) == false &&
	   it_channel->second.isVoiced(it_client->second.getNickname()) == false){
         addToClientBuffer(server, client_fd, ERR_CANNOTSENDTOCHAN(it_client->second.getNickname(), it_channel->second.getName()));
         return ;
      }
   }

   std::map<std::string, Client>::iterator member = it_channel->second.getClientList().begin();
   while (member != it_channel->second.getClientList().end())
   {
      if (member->second.getClientFd() != client_fd)
         addToClientBuffer(server, member->second.getClientFd(), RPL_PRIVMSG(it_client->second.getNickname(), it_client->second.getUsername(), target, msg_to_send));
      member++;
   }
}

static bool isUserinChannel(std::map<const int, Client>::iterator it_client, std::map<std::string, Channel>::iterator it_channel){
   if (it_channel->second.getClientList().find(it_client->second.getNickname()) != it_channel->second.getClientList().end())
      return (true);
   return (false);
}

void	privmsg(Server *server, int const client_fd, s_cmd cmd_infos){ 
   std::map<const int, Client>	client_list = server->getClients();
   std::map<std::string, Channel> channel_list = server->getChannels(); 
   std::map<const int, Client>::iterator it_client = client_list.find(client_fd);

   std::string target;
   std::string msg_to_send;
  
   size_t      delimiter = cmd_infos.message.rfind(":");
   if (delimiter == std::string::npos)
   {
      addToClientBuffer(server, client_fd, ERR_NOTEXTTOSEND(it_client->second.getNickname()));
      return ;
   }
   target = cmd_infos.message.substr(1, delimiter - 1);
   if (target.empty()){
      addToClientBuffer(server, client_fd, ERR_NORECIPIENT(it_client->second.getNickname()));
      return ;
   }
      
   msg_to_send = cmd_infos.message.substr(delimiter);

   delimiter = target.find(" ");
   if (delimiter != std::string::npos)
      target.erase(delimiter);  
   
   if (target[0] == '#'){
      std::map<std::string, Channel>::iterator it_channel = channel_list.find(target.substr(1));

      if (it_channel == channel_list.end())
         addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(it_client->second.getNickname(), target));
      else
         broadcastToChannel(server, client_fd, it_client, it_channel, target, msg_to_send);
   }
   else{
      std::map<std::string, Channel>::iterator it_channel = channel_list.find(target);
     
      std::map<const int, Client>::iterator it_target = client_list.begin();
      while (it_target!=client_list.end()){
         if (it_target->second.getNickname() == target)
             break;
         it_target++;
      }
      if (it_target == client_list.end() && it_channel == channel_list.end()){
         if (target == "raultbot")
            bot(server, client_fd, it_client, msg_to_send);
         else
            addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(it_client->second.getNickname(), target));   
      }
      else{
         if (it_target == client_list.end()){
            if (isUserinChannel(it_client, it_channel) == true){
               target.insert(1, "#"); 
               broadcastToChannel(server, client_fd, it_client, it_channel, target, msg_to_send);
            }
            else
               addToClientBuffer(server, client_fd, ERR_NOSUCHNICK(it_client->second.getNickname(), target));
         }
         else
            addToClientBuffer(server, it_target->first, RPL_PRIVMSG(it_client->second.getNickname(), it_client->second.getUsername(), target, msg_to_send));    
      }
   }
}