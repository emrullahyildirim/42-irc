#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include <iostream>
#include <map>

class Client;
class Command;
class Parser;

typedef std::map <std::string, Command> t_CommandsMap;

class CommandManager
{
    private:
        t_CommandsMap			_commands;
        
        CommandManager(const CommandManager& other);            
        CommandManager &operator=(const CommandManager& other);
    public:
		~CommandManager();
        CommandManager();
        
		const t_CommandsMap&	getCommands() const;

        void					registerCommand(const Command &cmd);
        void					executeCommand(Client &client, const Parser& parser);
        bool					isCommandExists(const std::string &name) const;
};

#endif