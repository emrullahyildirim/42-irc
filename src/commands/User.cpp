#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"
#include "../../headers/Parser.hpp"

void Command_User(Server &server, Client &client, const Parser& parser)
{
	if (client.getIsRegistered())
	{
        server.reply(client, 462, ":Unauthorized command (already registered)");
		return;
	}

	if (parser.getParams().size() < 3 || parser.getTrailing().empty()) {
        server.reply(client, 461, "USER :Not enough parameters");
        return ;
    }

	client.setUsername(parser.getParams()[0]);
    client.setRealname(parser.getTrailing());
    client.checkRegistration();
}