#include <sstream>
#include <cstdlib>
#include "../../headers/Client.hpp"
#include "../../headers/Server.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/Parser.hpp"

void Command_Mode(Server &server, Client &client, const Parser &parser)
{
	if (parser.getParams().empty()) {
		server.reply(client, 461, "MODE :Not enough parameters");
		return;
	}

	std::string target = parser.getParams()[0];

	if (target[0] == '#' || target[0] == '&') {
		Channel* channel = server.getChannelManager().getChannelByName(target);
		if (!channel) {
			server.reply(client, 403, target + " :No such channel");
			return;
		}

		if (parser.getParams().size() == 1) {
			std::string modes = channel->getModeString();
			if (modes.empty())
				server.reply(client, 324, target + " +");
			else
				server.reply(client, 324, target + " " + modes);
			return;
		}

		if (!channel->isClientInChannel(&client)) {
			server.reply(client, 442, target + " :You're not on that channel");
			return;
		}

		if (!channel->isOperator(&client)) {
			server.reply(client, 482, target + " :You're not channel operator");
			return;
		}

		std::string modeStr = parser.getParams()[1];
		bool adding = true;
		size_t paramIdx = 2;
		std::string appliedModes = "";
		std::string appliedParams = "";

		for (size_t i = 0; i < modeStr.length(); ++i) {
			char mode = modeStr[i];

			if (mode == '+') {
				adding = true;
				continue;
			}
			if (mode == '-') {
				adding = false;
				continue;
			}

			switch (mode) {
				case 'i':
					channel->setInviteOnly(adding);
					appliedModes += (adding ? "+i" : "-i");
					break;

				case 't':
					channel->setTopicRestricted(adding);
					appliedModes += (adding ? "+t" : "-t");
					break;

				case 'k':
					if (adding) {
						if (paramIdx >= parser.getParams().size()) {
							server.reply(client, 461, "MODE :Not enough parameters");
							continue;
						}
						std::string key = parser.getParams()[paramIdx++];
						channel->setPassword(key);
						appliedModes += "+k";
						appliedParams += " " + key;
					} else {
						channel->setPassword("");
						appliedModes += "-k";
					}
					break;

				case 'o':
					if (paramIdx >= parser.getParams().size()) {
						server.reply(client, 461, "MODE :Not enough parameters");
						continue;
					}
					{
						std::string nick = parser.getParams()[paramIdx++];
						Client* targetClient = server.getClientManager().getClientByNick(nick);
						if (!targetClient) {
							server.reply(client, 401, nick + " :No such nick/channel");
							continue;
						}
						if (!channel->isClientInChannel(targetClient)) {
							server.reply(client, 441, nick + " " + target + " :They aren't on that channel");
							continue;
						}
						if (adding)
							channel->addOperator(targetClient);
						else
							channel->removeOperator(targetClient);
						appliedModes += (adding ? "+o" : "-o");
						appliedParams += " " + nick;
					}
					break;

				case 'l':
					if (adding) {
						if (paramIdx >= parser.getParams().size()) {
							server.reply(client, 461, "MODE :Not enough parameters");
							continue;
						}
						std::string limitStr = parser.getParams()[paramIdx++];
						int limit = std::atoi(limitStr.c_str());
						if (limit <= 0) {
							continue;
						}
						channel->setLimit(static_cast<size_t>(limit));
						appliedModes += "+l";
						appliedParams += " " + limitStr;
					} else {
						channel->setLimit(0);
						appliedModes += "-l";
					}
					break;

				default:
					server.reply(client, 472, std::string(1, mode) + " :is unknown mode char to me");
					break;
			}
		}

		if (!appliedModes.empty()) {
			std::string modeMsg = ":" + client.getPrefix() + " MODE " + target + " " + appliedModes + appliedParams;
			channel->broadcast(modeMsg, NULL);
		}
	}
	else {
		if (target != client.getNickname()) {
			server.reply(client, 502, ":Cannot change mode for other users");
			return;
		}
		server.reply(client, 221, "+");
	}
}
