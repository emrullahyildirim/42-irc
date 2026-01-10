#include <iostream>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include "../headers/Server.hpp"

static bool isValidPort(const char *portStr)
{	
	for (int i = 0; portStr[i]; ++i)
		if (portStr[i] < '0' || portStr[i] > '9')
			return false;
	int port = std::atoi(portStr);
	return (port >= 1 && port <= 65535);
}

static bool isValidPassword(const char *password)
{	
	size_t len = std::strlen(password);
	if (len == 0 )
		return (false);
	for (size_t i = 0; i < len; ++i)
		if (password[i] == ' ' || password[i] == '\t' || 
			password[i] == '\r' || password[i] == '\n')
			return (false);
	return (true);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return (EXIT_FAILURE);
	}
	if (!isValidPort(argv[1]))
	{
		std::cerr << "Error: Invalid port. Port must be a number between 1 and 65535." << std::endl;
		return (EXIT_FAILURE);
	}
	if (!isValidPassword(argv[2]))
	{
		std::cerr << "Error: Invalid password. Password cannot be empty, contain spaces, or exceed 50 characters." << std::endl;
		return (EXIT_FAILURE);
	}
	
	signal(SIGINT, Server::signalHandler);
	signal(SIGTERM, Server::signalHandler);
	
	try {
		Server server("Manifest", std::atoi(argv[1]), argv[2]);
		server.Initialize();
		server.Run();	
	} catch (const std::exception &e) {
		std::cerr << "Server runtime error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
