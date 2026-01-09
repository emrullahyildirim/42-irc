#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include "Parser.hpp"

class Client;
class Server;

typedef void (*t_executeFunc)(Server &server, Client &client, const Parser &parser);

class Command
{
	private:
		std::string		_alias;
		std::string		_description;
		t_executeFunc	_executeFunc;
	public:
		~Command();
		Command();
		Command(const std::string &alias, const std::string &description, t_executeFunc t_executeFunc);
		Command(const Command& other);
		Command &operator=(const Command& other);

		const std::string &getAlias() const;
		const std::string &getDescription() const;

		void Execute(Client &client, const Parser& parser) const;
};

#endif