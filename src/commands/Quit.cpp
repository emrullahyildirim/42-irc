#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"
#include "../../headers/Parser.hpp"

void Command_Quit(Server &server, Client &client, const Parser &parser)
{
	std::string reason = parser.getTrailing();
	if (reason.empty())
		reason = "Client quit";
	
	std::string quitMsg = ":" + client.getPrefix() + " QUIT :" + reason;
	client.sendMessage("ERROR :Closing Link: " + client.getHostname() + " (" + reason + ")");
	
	server.disconnectClient(client);
}
