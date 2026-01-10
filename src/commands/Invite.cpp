#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/Parser.hpp"

void Command_Invite(Server &server, Client &client, const Parser &parser)
{
	if (parser.getParams().size() < 2) {
		server.reply(client, 461, "INVITE :Not enough parameters");
		return;
	}

	std::string targetNick = parser.getParams()[0];
	std::string channelName = parser.getParams()[1];

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

	if (channel->isClientInChannel(target)) {
		server.reply(client, 443, targetNick + " " + channelName + " :is already on channel");
		return;
	}

	channel->addInvite(targetNick);

	server.reply(client, 341, targetNick + " " + channelName);

	std::string inviteMsg = ":" + client.getPrefix() + " INVITE " + targetNick + " :" + channelName;
	target->sendMessage(inviteMsg);
}
