#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/Parser.hpp"

void Command_Topic(Server &server, Client &client, const Parser &parser)
{
	if (parser.getParams().empty()) {
		server.reply(client, 461, "TOPIC :Not enough parameters");
		return;
	}

	std::string channelName = parser.getParams()[0];

	Channel* channel = server.getChannelManager().getChannelByName(channelName);
	if (!channel) {
		server.reply(client, 403, channelName + " :No such channel");
		return;
	}

	if (!channel->isClientInChannel(&client)) {
		server.reply(client, 442, channelName + " :You're not on that channel");
		return;
	}

	if (parser.getTrailing().empty() && parser.getParams().size() == 1) {
		if (channel->getTopic().empty())
			server.reply(client, 331, channelName + " :No topic is set");
		else
			server.reply(client, 332, channelName + " :" + channel->getTopic());
		return;
	}

	if (channel->isTopicRestricted() && !channel->isOperator(&client)) {
		server.reply(client, 482, channelName + " :You're not channel operator");
		return;
	}

	std::string newTopic = parser.getTrailing();
	channel->setTopic(newTopic);

	std::string topicMsg = ":" + client.getPrefix() + " TOPIC " + channelName + " :" + newTopic;
	channel->broadcast(topicMsg, NULL);
}
