#include "../headers/Client.hpp"

Client::Client(Server &server, int socketFd) : _socketFd(socketFd), _isAuthenticated(false), _server(server) {}

Client& Client::operator=(const Client &other) {
	_socketFd = other._socketFd;
	_isAuthenticated = other._isAuthenticated;
	_username = other._username;
	_nickname = other._nickname;
	return *this;
}

int Client::getSocketFd() const
{
	return (_socketFd);
}

Server& Client::getServer() const
{
	return (_server);
}

bool Client::getIsAuthenticated() const
{
	return (_isAuthenticated);
}

std::string Client::getUsername() const
{
	return (_username);
}

std::string Client::getNickname() const
{
	return (_nickname);
}

void Client::authenticate(const std::string &username, const std::string &nickname)
{
	_username = username;
	_nickname = nickname;
	_isAuthenticated = true;
}