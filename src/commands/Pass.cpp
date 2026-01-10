#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"

void Command_Pass(Server &server, Client &client, const Parser &parser) {
	if (client.getIsRegistered())
	{
		server.reply(client, 462, ":Unauthorized command (already registered)");
		return;
	}
	
	std::string password;
	if (!parser.getParams().empty())
		password = parser.getParams()[0];
	else
		password = parser.getTrailing();

	if (password.empty()) {
		server.reply(client, 461, "PASS :Not enough parameters");
		return;
	}

    if (password != server.getPassword()) {
        server.reply(client, 464, ":Password incorrect.");
		server.disconnectClient(client);
		return ;
    }

    client.setAuthenticated(true);
	client.CheckRegisteration();
}