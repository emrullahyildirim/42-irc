#include "../headers/ChannelManager.hpp"
#include "../headers/Server.hpp"
#include "../headers/Client.hpp"

ChannelManager::~ChannelManager() {
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) 
	delete it->second;
}
ChannelManager::ChannelManager(Server &server) : _server(&server) {}

ChannelManager::ChannelManager(const ChannelManager& other) {
	*this = other;
}

ChannelManager& ChannelManager::operator=(const ChannelManager &other) {
	if (this != &other)
		for (std::map<std::string, Channel*>::const_iterator it = other._channels.begin(); it != other._channels.end(); ++it)
			_channels[it->first] = new Channel(*(it->second));
	return (*this);
}

Channel*	ChannelManager::createChannel(const std::string& name, Client &creator) {
	Channel* channel = new Channel(name);
	_channels[name] = channel;

	channel->addOperator(&creator);
	return channel;
}

void		ChannelManager::deleteChannel(const std::string& name) {
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it != _channels.end()) {
		delete it->second;
		_channels.erase(it);
	}
}

Channel*	ChannelManager::getChannelByName(const std::string& name) {
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it != _channels.end())
		return it->second;
	return NULL;
}

std::map<std::string, Channel*>&	ChannelManager::getChannels() { return _channels; }

void ChannelManager::joinChannel(const std::string& channelName, const std::string& key, Client &client) {
	Channel* channel = getChannelByName(channelName);
	
	if (!channel)
		channel = createChannel(channelName, client);
	else if (channel->isClientInChannel(&client))
		return ;
	else if (!channel->getPassword().empty() && key != channel->getPassword()) {
		_server->reply(client, 475, channelName + " :Cannot join channel (+k)");
		return;
	}

	channel->addClient(&client);
	std::string joinMsg = ":" + client.getPrefix() + " JOIN " + channelName;
	client.sendMessage(joinMsg);
    channel->broadcast(joinMsg, &client); 

	if (!channel->getTopic().empty())
    	_server->reply(client, 332, channelName + " :" + channel->getTopic());
		
	_server->reply(client, 353, "= " + channelName + " :" + channel->getClientListString());
	_server->reply(client, 366, channelName + " :End of /NAMES list.");
}

void ChannelManager::removeClientFromAllChannels(Client &client) {
	std::map<std::string, Channel*>::iterator it = _channels.begin();
	while (it != _channels.end()) {
		Channel* channel = it->second;
		if (channel->isClientInChannel(&client)) {
			std::string partMsg = ":" + client.getPrefix() + " PART " + channel->getName();
			channel->broadcast(partMsg, NULL);
			channel->removeClient(&client);
			channel->removeOperator(&client);
			
			if (channel->getClintCount() == 0) {
				std::string channelName = channel->getName();
				++it;
				deleteChannel(channelName);
			} else
				++it;
		} else 
			++it;
	}
}