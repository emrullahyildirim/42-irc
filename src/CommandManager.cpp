#include "../headers/CommandManager.hpp"
#include "../headers/Server.hpp"
#include "../headers/Client.hpp"

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

void CommandManager::executeCommand(Client &client, const Parser &parser)
{
	Server& server = client.getServer();
	const std::string	cmd = parser.getCommand();

	if (!client.getIsAuthenticated() && (cmd != "PASS" && cmd != "NICK" && cmd != "USER" && cmd != "CAP" && cmd != "PING") ) {
		server.reply(client, 464, ":Password required");
        return ;
    }

	if (!client.getIsRegistered() && (cmd != "PASS" && cmd != "NICK" && cmd != "USER" && cmd != "CAP" && cmd != "PING")) {
		server.reply(client, 451, ":You have not registered");
        return ;
    }
	
	std::map<std::string, Command>::const_iterator it = _commands.find(cmd);
	if (it == _commands.end()) {
		server.reply(client, 421, cmd + " :Unknown command");
		return ;
	}

	return (it->second.Execute(client, parser));
}

bool CommandManager::isCommandExists(const std::string &name) const
{
	return (_commands.find(name) != _commands.end());
}
