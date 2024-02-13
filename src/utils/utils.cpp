/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axel <axel@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 12:22:52 by axel              #+#    #+#             */
/*   Updated: 2024/02/10 12:22:52 by axel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/ircserv.hpp"

/**
 * 
 * @brief Returns the client associated with the given client file descriptor.
 * 
 * This function takes a Server object pointer and a client file descriptor as input.
 * It retrieves the map of clients associated with the server, then searches for the client
 * with the specified file descriptor within the map. If the client is found, a pointer to
 * the client object is returned; otherwise, it returns NULL.
 * 
 * @param server - pointer to the server object.
 * @param client_fd - file descriptor of the client to retrieve
 * @return `client*` pointer to the Client object if found.
*/
Client*	getClient(Server *server, int const client_fd)
{
	std::map<const int, Client>&		client_list = server->getClients();
	std::map<const int, Client>::iterator it_client = client_list.find(client_fd);
	
	if (it_client == client_list.end())
		return (NULL);
	return (&it_client->second);
}

/**
 * @brief Sends a server reply message to a client.
 * 
 * This function takes a client file descriptor and a message string as input,
 * then sends the message to the client using the send() function. It also
 * prints a log message indicating the message sent to the client.
 * 
 * @param client_fd The file descriptor of the client to send the message to.
 * @param client_buffer The message to send to the client.
*/
void	sendServerRpl(int const client_fd, std::string client_buffer) {
	std::istringstream	buf(client_buffer);
	std::string			reply;

	send(client_fd, client_buffer.c_str(), client_buffer.size(), 0);
	while (getline(buf, reply)) {
		std::cout << ToColor("[Server] Message sent to client #", Colors::Blue)
			<< client_fd << ToColor(reply, Colors::Blue) << std::endl;
	}
}
