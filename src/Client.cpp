#include <sys/socket.h>
#include "../headers/Client.hpp"

Client::~Client() {}

Client::Client(Server &server, int socketFd) : _socketFd(socketFd), _isAuthenticated(false), _isRegistered(false), _username("*"), _server(&server) {}

Client::Client(const Client& other) {
	*this = other;
}

Client& Client::operator=(const Client &other) {
    if (this != &other) {
        _socketFd = other._socketFd;
        _isAuthenticated = other._isAuthenticated;
		_isRegistered = other._isRegistered;
        _username = other._username;
        _nickname = other._nickname;
        _buffer = other._buffer;
		_server	= other._server;
    }
    return (*this);
}

int Client::getSocketFd() const { return _socketFd; }
bool Client::getIsAuthenticated() const { return _isAuthenticated; }
bool Client::getIsRegistered() const { return _isRegistered; }
const std::string& Client::getUsername() const { return _username; }
const std::string& Client::getNickname() const { return _nickname; }
const std::string& Client::getSocketBuffer() const { return _buffer; }
const std::string& Client::getHostname() const { return _hostname; }
Server& Client::getServer() const { return *_server; }

void Client::setNickname(std::string nickname) { _nickname = nickname; }
void Client::setAuthenticated(bool authenticated) { _isAuthenticated = authenticated; }
void Client::setRealname(std::string realname) { _realname = realname; }
void Client::setUsername(std::string username) { _username = username; }
void Client::setHostname(std::string hostname) { _hostname = hostname; }

void Client::appendBuffer(const std::string& str) {
    _buffer += str;
}

void Client::eraseBuffer(size_t pos, size_t len) {
    if (pos < _buffer.length()) {
        _buffer.erase(pos, len);
    }
}

void Client::sendMessage(std::string message)
{
	message += "\r\n";
	send(_socketFd, message.c_str(), message.length(), 0);
}


bool Client::isValidNickname(const std::string& nick) {
    if (nick.empty() || nick.length() > 9 || isdigit(nick[0]))
		return false;

    std::string special = "[]\\`_^{|}";
    for (size_t i = 0; i < nick.length(); ++i) {
        if (!isalnum(nick[i]) && special.find(nick[i]) == std::string::npos)
            return false;
    }
    return true;
}

void Client::CheckRegisteration()
{
	if (_isAuthenticated && !_nickname.empty() && _username.compare("*"))
	{
		_isRegistered = true;
		std::string welcomeMsg = ":Welcome to the Manifest Network, " + _nickname + "!" + _username + "@" + "localhost";
		_server->reply(*this, 1, welcomeMsg);
		_server->reply(*this, 2, ":Your host is " + _server->getName());
		_server->reply(*this, 3, ":This server was today.");
		_server->reply(*this, 4, _server->getName() + " 1.0 i o");
	}


}