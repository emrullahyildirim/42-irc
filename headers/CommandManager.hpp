#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include <iostream>
#include <map>
#include "Parser.hpp"
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
        void executeCommand(Client &client, const Parser& parser);
        bool isCommandExists(const std::string &name) const;
};

#endif