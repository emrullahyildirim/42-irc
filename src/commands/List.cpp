#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"
#include "../../headers/Channel.hpp"

void Command_List(Server &server, Client &client, const Parser &parser)
{
	server.reply(client, 321, "Channel :Users  Name");

	std::map<std::string, Channel*> channels = server.getChannelManager().getChannels();
    std::map<std::string, Channel*>::iterator it;

	for (it = channels.begin(); it != channels.end(); ++it) {
		Channel* channel = it->second;
		std::string channelName = channel->getName();
		size_t userCount = channel->getClintCount();
		std::string topic = channel->getTopic();
		server.reply(client, 322, channelName + " " + std::to_string(userCount) + " :" + topic);
	}
	server.reply(client, 323, ":End of /LIST");


}