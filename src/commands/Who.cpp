#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/Parser.hpp"

void Command_Who(Server &server, Client &client, const Parser &parser)
{
	if (parser.getParams().empty()) {
		server.reply(client, 315, "* :End of WHO list");
		return;
	}

	std::string target = parser.getParams()[0];

	if (target[0] == '#' || target[0] == '&') {
		Channel* channel = server.getChannelManager().getChannelByName(target);
		if (!channel) {
			server.reply(client, 315, target + " :End of WHO list");
			return;
		}

		const t_clientVector& clients = channel->getClients();
		for (size_t i = 0; i < clients.size(); ++i) {
			Client* member = clients[i];
			std::string flags = "H";
			if (channel->isOperator(member))
				flags += "@";
			
			std::string reply = target + " " + member->getUsername() + " " + 
				member->getHostname() + " " + server.getName() + " " + 
				member->getNickname() + " " + flags + " :0 " + member->getNickname();
			server.reply(client, 352, reply);
		}
		server.reply(client, 315, target + " :End of WHO list");
	}
	else {
		Client* targetClient = server.getClientManager().getClientByNick(target);
		if (!targetClient) {
			server.reply(client, 315, target + " :End of WHO list");
			return;
		}

		std::string reply = "* " + targetClient->getUsername() + " " + 
			targetClient->getHostname() + " " + server.getName() + " " + 
			targetClient->getNickname() + " H :0 " + targetClient->getNickname();
		server.reply(client, 352, reply);
		server.reply(client, 315, target + " :End of WHO list");
	}
}
