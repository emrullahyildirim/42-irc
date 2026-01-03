#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include <iostream>
#include <map>
#include "Command.hpp"
 
class Client;

class CommandManager
{
    private:
        std::map<std::string, Command> _commands;
    public:
		~CommandManager();
        CommandManager();
        CommandManager(const CommandManager& other);            
        CommandManager &operator=(const CommandManager& other);
        
        void registerCommand(const Command &cmd);
        bool executeCommand(const std::string &name, Client &client, const std::string &args);
        bool isCommandExists(const std::string &name) const;
};

#endif