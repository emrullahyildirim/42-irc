#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"

void Command_Cap(Server &server, Client &client, const Parser &parser) {
	(void) parser;
	client.sendMessage(":" + server.getName() + " CAP * LS :");
}