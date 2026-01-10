#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/Parser.hpp"

void Command_Privmsg(Server &server, Client &client, const Parser &parser) {
    if (parser.getParams().empty()) {
        server.reply(client, 411, ":No recipient given (PRIVMSG)");
        return;
    }
    if (parser.getTrailing().empty()) {
        server.reply(client, 412, ":No text to send");
        return;
    }

    std::string target = parser.getParams()[0];
    std::string message = parser.getTrailing();
    std::string formattedMsg = ":" + client.getPrefix() + " PRIVMSG " + target + " :" + message;

    if (target[0] == '#' || target[0] == '&') {
        Channel* channel = server.getChannelManager().getChannelByName(target);
        if (!channel) {
            server.reply(client, 403, target + " :No such channel");
            return;
        }
        if (!channel->isClientInChannel(&client)) {
            server.reply(client, 404, target + " :Cannot send to channel");
            return;
        }
        channel->broadcast(formattedMsg, &client);
    } 
    else {
        Client* targetClient = server.getClientManager().getClientByNick(target);
        if (!targetClient) {
            server.reply(client, 401, target + " :No such nick/channel");
            return;
        }
        targetClient->sendMessage(formattedMsg);
    }
}