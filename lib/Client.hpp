/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axel <axel@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 12:22:04 by axel              #+#    #+#             */
/*   Updated: 2024/02/14 11:58:28 by axel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

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

class Client {
	private:
		int				_client_fd;
		std::string		_readbuf;
		std::string		_sendbuf;
		bool			_to_deconnect;
		std::string		_nickname;
		std::string		_old_nickname;
		std::string 	_username;
		std::string		_realname;
		std::string		_mode;
		bool			_connexion_password;
		bool			_registrationDone;
		bool			_welcomeSent;
		bool			_hasAllInfo;
		int				_nbInfo;

	public:
		Client(int client_fd);
		~Client(void);
		
		int				getClientFd()const;
		std::string&	getSendBuffer();
		std::string&	getReadBuffer();
		std::string&	getNickname();
		std::string&	getOldNickname(); 
		std::string 	getUsername() const;
		std::string		getRealname() const;
		std::string&	getMode();
		bool&			getConnexionPassword();
		bool&			isRegistrationDone();
		bool&			isWelcomeSent();
		bool&			hasAllInfo();
		bool&			getDeconnexionStatus();
		int				getNbInfo() const;
		void			setReadBuffer(std::string const &buf);
		void			resetReadBuffer(std::string const &str);
		void			setSendBuffer(std::string const &buf);
		void			setDeconnexionStatus(bool status);
		void			setNickname(std::string const &nickname);
		void			setOldNickname(std::string const &nickname);
		void			setUsername(std::string const &username);
		void			setRealname(std::string const &realname);
		void			addMode(std::string const mode);
		void			removeMode(std::string const mode);
		void			setConnexionPassword(bool boolean);
		void			setRegistrationDone(bool boolean);
		void			setWelcomeSent(bool boolean);
		void			setHasAllInfo(bool boolean);
		void			setNbInfo(int n);

		int				isValid() const;
};

/****************************************************************
* 							FUNCTIONS							*
*****************************************************************/

#endif