#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/Parser.hpp"

void Command_Join(Server &server, Client &client, const Parser &parser)
{
    if (parser.getParams().empty()) {
        server.reply(client, 461, "JOIN :Not enough parameters");
        return;
    }

    std::string channelName = parser.getParams()[0];
    if (channelName[0] != '#' && channelName[0] != '&') {
        server.reply(client, 403, channelName + " :No such channel");
        return;
    }

    std::string key = "";
    if (parser.getParams().size() > 1)
        key = parser.getParams()[1];
    server.getChannelManager().joinChannel(channelName, key, client);
}