#include "../../headers/Server.hpp"
#include "../../headers/Client.hpp"
#include "../../headers/Parser.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/ChannelManager.hpp"

void Command_Part(Server &server, Client &client, const Parser &parser)
{
	if (!client.getIsRegistered()) {
		server.reply(client, 451, ":You have not registered");
		return;
	}

	const std::vector<std::string>& params = parser.getParams();
	if (params.empty()) {
		server.reply(client, 461, "PART :Not enough parameters");
		return;
	}

	std::string reason = "";
	if (params.size() > 1) {
		reason = params[1];
		if (!reason.empty() && reason[0] == ':')
			reason = reason.substr(1);
	}

	std::string channelList = params[0];
	std::string channelName;
	size_t pos = 0;
	
	while ((pos = channelList.find(',')) != std::string::npos || !channelList.empty()) {
		if (pos != std::string::npos) {
			channelName = channelList.substr(0, pos);
			channelList = channelList.substr(pos + 1);
		} else {
			channelName = channelList;
			channelList.clear();
		}
		
		if (channelName.empty())
			continue;

		Channel* channel = server.getChannelManager().getChannelByName(channelName);
		if (!channel) {
			server.reply(client, 403, channelName + " :No such channel");
			continue;
		}

		if (!channel->isClientInChannel(&client)) {
			server.reply(client, 442, channelName + " :You're not on that channel");
			continue;
		}

		server.getChannelManager().leaveChannel(channelName, client, reason);
	}
}
