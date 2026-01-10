#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include <algorithm>

Channel::~Channel() {}

Channel::Channel(std::string name) : _name(name), _topic(""), _password(""), _limit(0) {}

Channel::Channel(const Channel& other) {
	*this = other;
}

Channel& Channel::operator=(const Channel &other) {
	if (this != &other)
	{
		_name = other._name;
		_topic = other._topic;
		_password = other._password;
		_limit = other._limit;
		_clients = other._clients;
		_operators = other._operators;
	}
	return (*this);
}

std::string Channel::getName() const { return _name; }
std::string Channel::getPassword() const { return _password; }
std::string Channel::getTopic() const { return _topic; }
std::vector<Client*>& Channel::getClients() { return _clients; }
size_t Channel::getClintCount() const { return _clients.size(); }

void Channel::setTopic(const std::string& topic) { _topic = topic; }
void Channel::addClient(Client* client) { _clients.push_back(client); }
void Channel::removeClient(Client* client) {
	std::vector<Client*>::iterator it = std::find(_clients.begin(), _clients.end(), client);
	if (it != _clients.end())
		_clients.erase(it);
}

void Channel::broadcast(const std::string& message, Client* except) {
	for (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i] != except)
			_clients[i]->sendMessage(message);
	}
}

bool Channel::isOperator(Client* client) const {
	return std::find(_operators.begin(), _operators.end(), client) != _operators.end();
}

void Channel::addOperator(Client* client) {
	if (!isOperator(client))
		_operators.push_back(client);
}

void Channel::removeOperator(Client* client) {
	std::vector<Client*>::iterator it = std::find(_operators.begin(), _operators.end(), client);
	if (it != _operators.end())
		_operators.erase(it);
}

bool Channel::isClientInChannel(Client *client) const {
	return std::find(_clients.begin(), _clients.end(), client) != _clients.end();
}

std::string Channel::getClientListString() const {
	std::string clientList;
	for (size_t i = 0; i < _clients.size(); ++i) {
		if (isOperator(_clients[i]))
			clientList += "@";
		if (i > 0)
			clientList += " ";
		clientList += _clients[i]->getNickname();
	}
	return clientList;
}