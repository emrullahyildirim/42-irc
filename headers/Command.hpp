#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>

class Client;
class Server;
class Parser;

typedef void (*t_executeFunc)(Server &server, Client &client, const Parser &parser);

class Command
{
	private:
		const std::string	_alias;
		const std::string	_description;
		t_executeFunc		_executeFunc;
	public:
		~Command();
		Command(const std::string &alias, const std::string &description, t_executeFunc t_executeFunc);
		Command(const Command& other);
		Command &operator=(const Command& other);

		const std::string&	getAlias() const;
		const std::string&	getDescription() const;

		void				Execute(Client &client, const Parser& parser) const;
};

#endif