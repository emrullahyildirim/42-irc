#include "../headers/ChannelManager.hpp"
#include "../headers/Server.hpp"
#include "../headers/Client.hpp"

ChannelManager::~ChannelManager() {
	for (t_channelsMap::iterator it = _channels.begin(); it != _channels.end(); ++it) 
	delete it->second;
}
ChannelManager::ChannelManager(const Server &server) : _server(&server) {}

const t_channelsMap&	ChannelManager::getChannels() const { return _channels; }

Channel*	ChannelManager::getChannelByName(const std::string& name) const {
	t_channelsMap::const_iterator it = _channels.find(name);
	if (it != _channels.end())
		return (it->second);
	return (NULL);
}

Channel*	ChannelManager::createChannel(const std::string& name, Client &creator) {
	if (_channels.find(name) != _channels.end())
        return _channels[name];
	Channel* channel = new Channel(name);
	_channels[name] = channel;

	channel->addOperator(&creator);
	return (channel);
}

void	ChannelManager::deleteChannel(const std::string& name) {
	t_channelsMap::iterator it = _channels.find(name);
	if (it != _channels.end()) {
		delete it->second;
		_channels.erase(it);
	}
}

void ChannelManager::joinChannel(const std::string& channelName, const std::string& key, Client &client) {
	Channel* channel = getChannelByName(channelName);
	
	if (!channel) {
		channel = createChannel(channelName, client);
	} else {
		if (channel->isClientInChannel(&client))
			return;
		
		if (channel->isInviteOnly() && !channel->isInvited(client.getNickname())) {
			_server->reply(client, 473, channelName + " :Cannot join channel (+i)");
			return;
		}
		
		if (!channel->getPassword().empty() && key != channel->getPassword()) {
			_server->reply(client, 475, channelName + " :Cannot join channel (+k)");
			return;
		}
		
		if (channel->getLimit() > 0 && channel->getClientCount() >= channel->getLimit()) {
			_server->reply(client, 471, channelName + " :Cannot join channel (+l)");
			return;
		}
	}

	channel->addClient(&client);
	channel->removeInvite(client.getNickname());
	std::string joinMsg = ":" + client.getPrefix() + " JOIN " + channelName;
	client.sendMessage(joinMsg);
    channel->broadcast(joinMsg, &client); 

	if (!channel->getTopic().empty())
    	_server->reply(client, 332, channelName + " :" + channel->getTopic());
	else 
		_server->reply(client, 331, channelName + " No topic is set");
	_server->reply(client, 353, "= " + channelName + " :" + channel->getClientListString());
	_server->reply(client, 366, channelName + " :End of /NAMES list");
}

void ChannelManager::leaveChannel(const std::string& channelName, Client &client, const std::string& reason) {
	Channel* channel = getChannelByName(channelName);
	if (!channel)
		return;
	
	if (!channel->isClientInChannel(&client))
		return;
	
	std::string partMsg = ":" + client.getPrefix() + " PART " + channelName;
	if (!reason.empty())
		partMsg += " :" + reason;
	
	channel->broadcast(partMsg, NULL);
	channel->removeClient(&client);
	
	if (channel->getClientCount() == 0)
		deleteChannel(channelName);
}

void ChannelManager::removeClientFromAllChannels(Client &client) {
	std::string quitMsg = ":" + client.getPrefix() + " QUIT :Client disconnected";
	
	std::vector<std::string> channelsToDelete;
	for (t_channelsMap::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		Channel* channel = it->second;
		if (channel->isClientInChannel(&client)) {
			channel->broadcast(quitMsg, &client);
			channel->removeClient(&client);
			if (channel->getClientCount() == 0)
				channelsToDelete.push_back(it->first);
		}
	}
	
	for (size_t i = 0; i < channelsToDelete.size(); ++i)
		deleteChannel(channelsToDelete[i]);
}