#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

typedef std::vector<std::string> t_paramsVector;

class Parser
{
	private:
		std::string					_raw;
		std::string					_command;
		t_paramsVector				_params;
		std::string					_trailing;
	public:
		~Parser();
		Parser(std::string raw);
		Parser(const Parser& raw);
		Parser &operator=(const Parser& raw);

		const std::string				&getRaw() const;
		const std::string				&getCommand() const;
		const t_paramsVector			&getParams() const;
		const std::string				&getTrailing() const;
};

#endif