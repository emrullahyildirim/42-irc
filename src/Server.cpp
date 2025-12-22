#include "../headers/Server.hpp"

Server* Server::_instance = NULL;

Server::Server()
{
}

Server& Server::getInstance()
{
	if (!_instance)
		_instance = new Server();
	return	*_instance;
}