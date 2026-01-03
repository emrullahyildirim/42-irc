#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include <iostream>
#include <map>
#include "Command.hpp"

class CommandManager
{
    private:
        std::map<std::string, Command> _commands;
    public:
        CommandManager();
        void registerCommand(const Command &cmd);
        bool executeCommand(const std::string &name, Client &client, const std::string &args);
        bool isCommandExists(const std::string &name) const;
};

#endif