#include "../../lib/Client.hpp"

							/*****************************
							 * CONSTRUCTOR & DESTRUCTOR *
							*****************************/

Client::Client(int client_fd): _client_fd(client_fd - 3){
	/*std::cout << ToColor("Client constructor for Client #", Colors::Yellow)
		<< client_fd << std::endl;
	*/
}

Client::~Client(void){
}

							/********************
							 * GETTER & SETTER *
							*********************/

int				Client::getClientFd() const { return (_client_fd); }
std::string&	Client::getSendBuffer()  	{ return (_sendbuf); }
std::string&	Client::getReadBuffer()  	{ return (_readbuf); }
std::string&	Client::getNickname()  		{ return (_nickname); }
std::string&	Client::getOldNickname()  	{ return (_old_nickname); }
std::string 	Client::getUsername() const { return (_username); }
std::string		Client::getRealname() const { return (_realname); }
std::string&	Client::getMode()			{ return (_mode); }

bool&			Client::getConnexionPassword()	{ return (_connexion_password); }
bool&			Client::isRegistrationDone() 	{ return (_registrationDone); }
bool&			Client::isWelcomeSent()			{ return (_welcomeSent); }
bool&			Client::hasAllInfo() 			{ return (_hasAllInfo); }
bool&			Client::getDeconnexionStatus()	{ return (_to_deconnect); }
int				Client::getNbInfo() const 		{ return (_nbInfo); }

void	Client::setReadBuffer(std::string const &buf) {
	_readbuf += buf;
}

void	Client::resetReadBuffer(std::string const &str){
	_readbuf.clear();
	_readbuf = str;
}

void	Client::setSendBuffer(std::string const &buf){
	_sendbuf += buf;
}

void	Client::setDeconnexionStatus(bool status){
	_to_deconnect = status;
}

void	Client::setNickname(std::string const &nickname){
	_nickname = (_nickname.size() > 9) ? nickname.substr(0, 9) : nickname;
}

void	Client::setOldNickname(std::string const &nickname){
	_old_nickname = nickname;
}

void	Client::setUsername(std::string const &username){
	_username = username;
}

void	Client::setRealname(std::string const &realname){
	_realname = realname;
}

void	Client::addMode(std::string const mode){
	if (_mode.empty() == true)
		_mode = "+" + mode;
	else
		_mode += mode;
}

void	Client::removeMode(std::string const mode){
	size_t pos = _mode.find(mode);
	_mode.erase(pos, 1);
}

void	Client::setConnexionPassword(bool boolean){
	_connexion_password = boolean;
}

void	Client::setRegistrationDone(bool boolean){
	_registrationDone = boolean;
}

void	Client::setWelcomeSent(bool boolean){
	_welcomeSent = boolean;
}

void	Client::setHasAllInfo(bool boolean){
	_hasAllInfo = boolean;
}

void	Client::setNbInfo(int n){
	_nbInfo += n;
	if (_nbInfo <= 0)
		_nbInfo = 0;
}

							/**********************
							 *		FUNCTIONS	 *
							***********************/
/**
 * @brief Checks if the client's information is valid.
 * 
 * This function validates the client's information by ensuring that the username,
 * nickname, real name, connection password, and the total number of information
 * fields are not empty or false. It returns SUCCESS if all conditions are met,
 * otherwise FAILURE.
 * 
 * @return int Returns SUCCESS if the client's information is valid, otherwise FAILURE.
 */
int	Client::isValid() const{ 
	if (_username.empty())
		return (FAILURE);
	if (_nickname.empty())
		return (FAILURE);
	if (_realname.empty())
		return (FAILURE);
	if (_connexion_password == false)
		return (FAILURE);
	if (this->getNbInfo() < 3)
		return (FAILURE);
	return (SUCCESS);
}