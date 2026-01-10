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
		std::string		_alias;
		t_executeFunc	_executeFunc;
	public:
		~Command();
		Command();
		Command(const std::string &alias, t_executeFunc executeFunc);
		Command(const Command& other);
		Command &operator=(const Command& other);

		const std::string&	getAlias() const;
		void				Execute(Client &client, const Parser& parser) const;
};

#endif