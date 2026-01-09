#include "../headers/Client.hpp"
#include "../headers/Server.hpp"
#include "../headers/Commands.hpp"

void Command_Cap(Server &server, Client &client, const Parser &parser) {
	(void) server;
	(void) parser;
	std::string response = ":" + server.getName() + " CAP * LS :";
	client.sendMessage(response);
}

void Command_Ping(Server &server, Client &client, const Parser &parser) {
    if (parser.getParams().empty() && parser.getTrailing().empty())
        server.reply(client, 409, ":No origin specified");

    std::string token;
    if (!parser.getTrailing().empty())
        token = parser.getTrailing();
    else
        token = parser.getParams()[0];

    std::string response = ":" + server.getName() + " PONG " + server.getName() + " :" + token;
    
    client.sendMessage(response);
}

void Command_Pass(Server &server, Client &client, const Parser &parser) {
	std::cout << "sa" << std::endl;
    if (client.getIsAuthenticated())
	{
        server.reply(client, 462, ":Unauthorized command (already registered)");
		return;
	}

    if (parser.getTrailing().empty())
	{
        server.reply(client, 461, "PASS :Not enough parameters");
		return;
	}

    if (parser.getTrailing() != server.getPassword()) {
        server.reply(client, 464, ":Password incorrect.");
		server.disconnectClient(client);
    }

    client.setAuthenticated(true);
	client.CheckRegisteration();
}

void Command_Nick(Server &server, Client &client, const Parser &parser)
{
	if (parser.getParams().empty()) {
		server.reply(client, 431, ":No nickname given");
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

	if (client.getIsRegistered())
		client.sendMessage(":" + oldNick + " NICK " + nick);

	client.CheckRegisteration();
}

void Command_User(Server &server, Client &client, const Parser& parser)
{
	if (parser.getParams().size() < 3) {
        server.reply(client, 461, "USER :Not enough parameters");
        return ;
    }
	client.setUsername(parser.getParams()[0]);
    client.setRealname(parser.getTrailing());
    client.CheckRegisteration();
    return ;
}