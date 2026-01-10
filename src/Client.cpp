#include <sys/socket.h>
#include "../headers/Client.hpp"
#include "../headers/Server.hpp"

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
		_realname = other._realname;
        _nickname = other._nickname;
		_hostname = other._hostname;
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
const std::string& Client::getBuffer() const { return _buffer; }
const std::string& Client::getHostname() const { return _hostname; }
Server& Client::getServer() const { return *_server; }

void Client::setAuthenticated(bool authenticated) { _isAuthenticated = authenticated; }
void Client::setNickname(const std::string &nickname) { _nickname = nickname; }
void Client::setRealname(const std::string &realname) { _realname = realname; }
void Client::setUsername(const std::string &username) { _username = username; }
void Client::setHostname(const std::string &hostname) { _hostname = hostname; }

void Client::appendBuffer(const std::string& str) {
    _buffer += str;
}

void Client::eraseBuffer(size_t pos, size_t len) {
    if (pos < _buffer.length())
        _buffer.erase(pos, len);
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
	if (_isRegistered)
    	return;
	if (_isAuthenticated && !_nickname.empty() && _username.compare("*"))
	{
		_isRegistered = true;
		_server->reply(*this, 1, ":Welcome to the Manifest Network, " + _nickname + "!" + _username + "@" + _hostname);
		_server->reply(*this, 2, ":Your host is " + _server->getName());
		_server->reply(*this, 3, ":This server was created today.");
		_server->reply(*this, 4, ":" + _server->getName() + " 1.0 i o oiws obtkmlvsn");
	}
}

std::string Client::getPrefix() const
{
	return (_nickname + "!" + _username + "@" + _hostname);
}
