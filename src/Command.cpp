#include "../headers/Command.hpp"
#include "../headers/Client.hpp"

Command::~Command() {}

Command::Command() : _executeFunc(NULL) {}

Command::Command(const std::string &alias, const std::string &description, t_executeFunc executeFunc)
	: _alias(alias), _description(description), _executeFunc(executeFunc) {}

Command::Command(const Command& other) {
	*this = other;
}

Command& Command::operator=(const Command& other) {
	if (this != &other) {
		_alias = other._alias;
		_description = other._description;
		_executeFunc = other._executeFunc;
	}
	return (*this);
}

const std::string &Command::getAlias() const { return (_alias); }	
const std::string &Command::getDescription() const { return (_description); }

void Command::Execute(Client &client, const Parser &parser) const {
	if (_executeFunc)
		_executeFunc(client.getServer(), client, parser);
}
