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

	Server server = Server("Manifest", atoi(argv[1]), argv[2]);
	try {
		server.Initialize();
		server.Run();
	} catch (const std::exception &e) {
		std::cerr << "Server runtime error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}