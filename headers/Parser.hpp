#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

class Parser
{
	private:
		std::string					_raw;
		std::string					_command;
		std::vector<std::string>	_params;
		std::string					_trailing;
	public:
		~Parser();
		Parser(std::string raw);
		Parser(const Parser& raw);
		Parser &operator=(const Parser& raw);

		const std::string	&getRaw() const;
		const std::string	&getCommand() const;
		const std::vector<std::string> &getParams() const;
		const std::string	&getTrailing() const;
};

#endif