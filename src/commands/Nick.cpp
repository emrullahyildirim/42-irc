#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"
#include "../../headers/Parser.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/ChannelManager.hpp"

void Command_Nick(Server &server, Client &client, const Parser &parser)
{
	if (parser.getParams().empty()) {
		server.reply(client, 431, "NICK :No nickname given");
		return ;
	}
	std::string nick = parser.getParams()[0];
	if (!Client::isValidNickname(nick))
	{
		server.reply(client, 432, nick + " :Erroneous nickname");
        return ;
	}

	if (server.isNickInUse(nick)) {
        server.reply(client, 433, nick + " :Nickname is already in use");
        return ;
    }
	
	std::string oldNick = client.getNickname();
	std::string nickMsg = ":" + oldNick + "!" + client.getUsername() + "@" + client.getHostname() + " NICK :" + nick;
	
	client.setNickname(nick);

	bool wasRegistered = client.getIsRegistered();
	client.checkRegistration();
	
	if (wasRegistered) {
		client.sendMessage(nickMsg);
		
		const t_channelsMap& channels = server.getChannelManager().getChannels();
		for (t_channelsMap::const_iterator it = channels.begin(); it != channels.end(); ++it) {
			Channel* channel = it->second;
			if (channel->isClientInChannel(&client))
				channel->broadcast(nickMsg, &client);
		}
	}
}