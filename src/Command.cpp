#include "../headers/Command.hpp"

Command::Command() : _alias(""), _description(""), _executeFunc(NULL) {}

Command::Command(const std::string &alias, const std::string &description, t_executeFunc executeFunc)
	: _alias(alias), _description(description), _executeFunc(executeFunc) {}

Command::Command(const Command& other) {
    *this = other;
}

Command& Command::operator=(const Command& other) {
    if (this != &other) {
        this->_alias = other._alias;
        this->_description = other._description;
        this->_executeFunc = other._executeFunc;
    }
    return *this;
}

const std::string &Command::getAlias() const{ return (_alias); }	
const std::string &Command::getDescription() const { return (_description); }

bool Command::Execute(Client &client, const std::string &args) const
{
	if (!_executeFunc)
		return (false);
	return (_executeFunc(client, args));
}
