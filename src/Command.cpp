#include "../headers/Command.hpp"

Command::Command(const std::string &alias, const std::string &description, bool (*executeFunc)(Client &client, const std::string &args))
	: _alias(alias), _description(description), _executeFunc(executeFunc) {}

std::string Command::getAlias() const
{	
	return (_alias);
}	

std::string Command::getDescription() const
{
	return (_description);
}

bool Command::Execute(Client &client, const std::string &args) const
{
	if (!_executeFunc)
		return (false);
	return (_executeFunc(client, args));
}
