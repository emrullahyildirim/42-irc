#include "../headers/Command.hpp"
#include "../headers/Client.hpp"

Command::~Command() {}

Command::Command() : _executeFunc(NULL) {}

Command::Command(const std::string &alias, t_executeFunc executeFunc)
	: _alias(alias), _executeFunc(executeFunc) {}

Command::Command(const Command& other) {
    *this = other;
}

Command& Command::operator=(const Command& other) {
	if (this != &other) {
		_alias = other._alias;
		_executeFunc = other._executeFunc;
	}
    return (*this);
}

const std::string &Command::getAlias() const { return (_alias); }

void Command::Execute(Client &client, const Parser &parser) const {
	if (_executeFunc)
		_executeFunc(client.getServer(), client, parser);
}
