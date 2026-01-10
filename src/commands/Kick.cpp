#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/Parser.hpp"

void Command_Kick(Server &server, Client &client, const Parser &parser)
{
	if (parser.getParams().size() < 2) {
		server.reply(client, 461, "KICK :Not enough parameters");
		return;
	}

	std::string channelName = parser.getParams()[0];
	std::string targetNick = parser.getParams()[1];
	std::string reason = parser.getTrailing().empty() ? "No reason given" : parser.getTrailing();
	
	Channel* channel = server.getChannelManager().getChannelByName(channelName);
	if (!channel) {
		server.reply(client, 403, channelName + " :No such channel");
		return;
	}

	if (!channel->isClientInChannel(&client)) {
		server.reply(client, 442, channelName + " :You're not on that channel");
		return;
	}

	if (!channel->isOperator(&client)) {
		server.reply(client, 482, channelName + " :You're not channel operator");
		return;
	}

	Client* target = server.getClientManager().getClientByNick(targetNick);
	if (!target) {
		server.reply(client, 401, targetNick + " :No such nick/channel");
		return;
	}

	if (!channel->isClientInChannel(target)) {
		server.reply(client, 441, targetNick + " " + channelName + " :They aren't on that channel");
		return;
	}

	std::string kickMsg = ":" + client.getPrefix() + " KICK " + channelName + " " + targetNick + " :" + reason;
	channel->broadcast(kickMsg, NULL);

	channel->removeClient(target);

	if (channel->getClientCount() == 0)
		server.getChannelManager().deleteChannel(channelName);
}
