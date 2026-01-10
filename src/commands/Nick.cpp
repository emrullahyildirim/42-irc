#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"
#include "../../headers/Parser.hpp"

void Command_Nick(Server &server, Client &client, const Parser &parser)
{
	if (parser.getParams().empty()) {
		server.reply(client, 431, "NICK :No nickname given");
		return ;
	}
	std::string nick = parser.getParams()[0];
	if (!client.isValidNickname(nick))
	{
		server.reply(client, 432, nick + " :Erroneous nickname");
        return ;
	}

	if (server.isNickInUse(nick)) {
        server.reply(client, 433, nick + " :Nickname is already in use");
        return ;
    }
	
	std::string oldNick = client.getNickname();
	client.setNickname(nick);

	bool isRegistered = client.getIsRegistered();
	client.checkRegistration();
	if (isRegistered)
		client.sendMessage(":" + oldNick + "!" + client.getUsername() + "@" + client.getHostname() + " NICK :" + nick);
}