#include "../headers/CommandManager.hpp"

CommandManager::~CommandManager() {}

CommandManager::CommandManager() {}

CommandManager::CommandManager(const CommandManager& other) {
	*this = other;
}

CommandManager& CommandManager::operator=(const CommandManager& other) {
	if (this != &other)
		_commands = other._commands;
	return (*this);
}

void CommandManager::registerCommand(const Command &cmd)
{
	_commands[cmd.getAlias()] = cmd;
}

bool CommandManager::executeCommand(const std::string &name, Client &client, const std::string &args)
{
	std::map<std::string, Command>::const_iterator it = _commands.find(name);
	if (it == _commands.end())
		return (false);
	return (it->second.Execute(client, args));
}

bool CommandManager::isCommandExists(const std::string &name) const
{
	return (_commands.find(name) != _commands.end());
}
