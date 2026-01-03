#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include "Client.hpp"

class Command
{
	private:
		std::string _alias;
		std::string _description;
		bool (*_executeFunc)(Client &client, const std::string &args);
	public:
		Command(const std::string &alias, const std::string &description, bool (*executeFunc)(Client &client, const std::string &args));
		std::string getAlias() const;
		std::string getDescription() const;

		bool Execute(Client &client, const std::string &args) const;
};

#endif