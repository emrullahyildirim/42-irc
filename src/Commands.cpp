#include "../headers/Client.hpp"
#include "../headers/Server.hpp"

bool Command_Ping(Client &client, const Parser &parser) {
	Server &server = client.getServer();
    if (parser.getParams().empty() && parser.getTrailing().empty()) {
        server.reply(client, 409, ":No origin specified");
        return false;
    }

    std::string token;
    if (!parser.getTrailing().empty())
        token = parser.getTrailing();
    else
        token = parser.getParams()[0];

    std::string response = ":" + server.getName() + " PONG " + server.getName() + " :" + token;
    
    client.sendMessage(response);
    return true;
}