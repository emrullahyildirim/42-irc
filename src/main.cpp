#include <iostream>
#include <cstdlib>

#include "../headers/Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv <server port> <server password>" << std::endl;
		return (EXIT_FAILURE);
	}

	Server server = Server(atoi(argv[1]), argv[2]);
	server.Initialize();
	server.Run();

	return (EXIT_SUCCESS);
}