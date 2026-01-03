#include "../headers/Client.hpp"

bool Command_Pass(Client &client, const std::string &args)
{
	if (client.getIsAuthenticated())
	{
		std::cout << "Client " << client.getSocketFd() << " is already authenticated." << std::endl;
		return (false);
	}
	if (args.empty())
	{
		std::cout << "PASS command requires a password argument." << std::endl;
		return (false);
	}
	Server &server = client.getServer();

	if (args != server.getPassword())
	{
		std::cout << "Client " << client.getSocketFd() << " provided an incorrect password." << std::endl;
		return (false);
	}
	client.authenticate("defaultUser", "defaultNick");
	std::cout << "Client " << client.getSocketFd() << " authenticated successfully." << std::endl;

	return (true);
}