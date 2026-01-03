#include "../headers/Client.hpp"

Client::~Client() {}

Client::Client(Server &server, int socketFd) : _socketFd(socketFd), _isAuthenticated(false), _server(&server) {}

Client::Client(const Client& other) {
	*this = other;
}

Client& Client::operator=(const Client &other) {
    if (this != &other) {
        _socketFd = other._socketFd;
        _isAuthenticated = other._isAuthenticated;
        _username = other._username;
        _nickname = other._nickname;
        _buffer = other._buffer;
		_server	= other._server;
    }
    return (*this);
}

int Client::getSocketFd() const { return _socketFd; }
bool Client::getIsAuthenticated() const { return _isAuthenticated; }
const std::string& Client::getUsername() const { return _username; }
const std::string& Client::getNickname() const { return _nickname; }
const std::string& Client::getSocketBuffer() const { return _buffer; }
Server& Client::getServer() const { return *_server; }

void Client::appendBuffer(const std::string& str) {
    _buffer += str;
}

void Client::eraseBuffer(size_t pos, size_t len) {
    if (pos < _buffer.length()) {
        _buffer.erase(pos, len);
    }
}

void Client::authenticate(const std::string &username, const std::string &nickname) {
    _username = username;
    _nickname = nickname;
    _isAuthenticated = true;
}