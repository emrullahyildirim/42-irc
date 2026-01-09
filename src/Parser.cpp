#include "../headers/Parser.hpp"

Parser::~Parser() {}

Parser::Parser(std::string raw) : _raw(raw)
{	
	std::string	head(raw);
	size_t	trailingPos = raw.find(" :");
	if (trailingPos != std::string::npos)
	{	
		head = raw.substr(0, trailingPos);
		_trailing = raw.substr(trailingPos + 2);
	}

	while (1)
	{
		size_t	spacePos = head.find(' ');
		if (spacePos == std::string::npos)
			break;
		
		std::string param = head.substr(0, spacePos);
		if (!param.empty())
			_params.push_back(param);
		head = head.substr(spacePos + 1);
	}
	
	if (!head.empty())
		_params.push_back(head);

	if (_params.empty())
		return ;

	_command = _params[0];
	_params.erase(_params.begin());
	for (size_t i = 0; i < _command.length(); ++i)
		_command[i] = std::toupper(_command[i]);
}

Parser::Parser(const Parser& other) 
{
	*this = other;
}

Parser &Parser::operator=(const Parser& other)
{
	if (this != &other)
	{
		_raw = other._raw;
		_command = other._command;
		_params = other._params;
		_trailing = other._trailing;
	}
	return (*this);
}

const std::string &Parser::getRaw() const {return _raw;}
const std::string &Parser::getCommand() const {return _command;}
const std::string &Parser::getTrailing() const {return _trailing;}
const std::vector<std::string> &Parser::getParams() const {return _params;}