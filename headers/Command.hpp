#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include "Client.hpp"

typedef bool (*t_executeFunc)(Client &client, const std::string &args);

class Command
{
	private:
		std::string		_alias;
		std::string		_description;
		t_executeFunc	_executeFunc;
	public:
		~Command();
		Command();
		Command(const std::string &alias, const std::string &description, bool (*executeFunc)(Client &client, const std::string &args));
		Command(const Command& other);
		Command &operator=(const Command& other);

		const std::string &getAlias() const;
		const std::string &getDescription() const;

		bool Execute(Client &client, const std::string &args) const;
};

#endif