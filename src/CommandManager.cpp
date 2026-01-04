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

bool CommandManager::executeCommand(Client &client, const Parser &parser)
{
	std::map<std::string, Command>::const_iterator it = _commands.find(parser.getCommand());
	if (it == _commands.end())
		return (false);
	return (it->second.Execute(client, parser));
}

bool CommandManager::isCommandExists(const std::string &name) const
{
	return (_commands.find(name) != _commands.end());
}
