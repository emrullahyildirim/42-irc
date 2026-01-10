#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include <algorithm>
#include <sstream>

Channel::~Channel() {}
Channel::Channel(const std::string &name) 
: _name(name), _topic(""), _password(""), _limit(0), _inviteOnly(false), _topicRestricted(false) {}

const std::string &Channel::getName() const { return (_name); }
const std::string &Channel::getPassword() const { return (_password); }
const std::string &Channel::getTopic() const { return (_topic); }
const t_clientVector& Channel::getClients() const { return (_clients); }
size_t Channel::getLimit() const { return (_limit); }
bool Channel::isInviteOnly() const { return (_inviteOnly); }
bool Channel::isTopicRestricted() const { return (_topicRestricted); }

void Channel::setName(const std::string &name) { _name = name; }
void Channel::setPassword(const std::string& password) { _password = password; }
void Channel::setTopic(const std::string& topic) { _topic = topic; }
void Channel::setLimit(size_t limit) { _limit = limit; }
void Channel::setInviteOnly(bool value) { _inviteOnly = value; }
void Channel::setTopicRestricted(bool value) { _topicRestricted = value; }

void Channel::addClient(Client* client) {
	if (!isClientInChannel(client)) 
			_clients.push_back(client);
}

void Channel::removeClient(Client* client) {
	removeOperator(client);
	
	t_clientVector::iterator it = std::find(_clients.begin(), _clients.end(), client);
	if (it != _clients.end())
		_clients.erase(it);
}

bool Channel::isClientInChannel(Client *client) const {
	return (std::find(_clients.begin(), _clients.end(), client) != _clients.end());
}

size_t Channel::getClientCount() const { 
	return (_clients.size()); 
}

bool Channel::isOperator(Client* client) const {
	return (std::find(_operators.begin(), _operators.end(), client) != _operators.end());
}

void Channel::addOperator(Client* client) {
	if (!isOperator(client))
		_operators.push_back(client);
}

void Channel::removeOperator(Client* client) {
	t_clientVector::iterator it = std::find(_operators.begin(), _operators.end(), client);
	if (it != _operators.end())
		_operators.erase(it);
}

bool Channel::isInvited(const std::string& nickname) const {
	t_inviteMap::const_iterator it = _invited.find(nickname);
	return (it != _invited.end() && it->second);
}

void Channel::addInvite(const std::string& nickname) {
	_invited[nickname] = true;
}

void Channel::removeInvite(const std::string& nickname) {
	_invited.erase(nickname);
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
	return (clientList);
}

void Channel::broadcast(const std::string& message, Client* except) {
	for (size_t i = 0; i < _clients.size(); ++i)
		if (_clients[i] != except)
			_clients[i]->sendMessage(message);
}

std::string Channel::getModeString() const {
	std::string modes = "+";
	std::string params = "";
	
	if (_inviteOnly)
		modes += "i";
	if (_topicRestricted)
		modes += "t";
	if (!_password.empty()) {
		modes += "k";
		params += " " + _password;
	}
	if (_limit > 0) {
		modes += "l";
		std::stringstream ss;
		ss << _limit;
		params += " " + ss.str();
	}
	
	if (modes == "+")
		return ("");
	return (modes + params);
}