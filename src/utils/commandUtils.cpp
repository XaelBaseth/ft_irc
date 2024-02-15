#include "../../lib/ircserv.hpp"

/**
 * @brief Extracts the reason from a message.
 * 
 * This function takes a message as input and extracts the reason part after the ':' character.
 * 
 * @param msg_to_parse The message from which to extract the reason.
 * @return The reason extracted from the message.
 */
std::string	getReason(std::string msg_to_parse)
{
	std::string reason;
	
	reason.clear();
	
	if (msg_to_parse.find(":") != msg_to_parse.npos) 
		reason.append(msg_to_parse, msg_to_parse.find(":") + 1, std::string::npos);
	return (reason);
}