#include <sstream>
#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/Parser.hpp"
#include "../../headers/ChannelManager.hpp"

void Command_List(Server &server, Client &client, const Parser &parser)
{
	(void)parser;
	
	if (!client.getIsRegistered()) {
		server.reply(client, 451, ":You have not registered");
		return;
	}

	server.reply(client, 321, "Channel :Users  Name");

	const t_channelsMap& channels = server.getChannelManager().getChannels();
	for (t_channelsMap::const_iterator it = channels.begin(); it != channels.end(); ++it) {
		Channel* channel = it->second;
		std::stringstream ss;
		ss << channel->getClientCount();
		server.reply(client, 322, channel->getName() + " " + ss.str() + " :" + channel->getTopic());
	}
	
	server.reply(client, 323, ":End of /LIST");
}