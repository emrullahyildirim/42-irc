#ifndef CHANNELMANAGER_HPP
#define CHANNELMANAGER_HPP
#include <string>
#include <map>
#include "Channel.hpp"

class Client;
class Server;

typedef	std::map<std::string, Channel*> t_channelsMap;

class ChannelManager 
{
	private:
		const Server*		_server;
		t_channelsMap		_channels;
	public:
		~ChannelManager();
		ChannelManager(const Server &server);
		ChannelManager(const ChannelManager& other);
		ChannelManager& operator=(const ChannelManager &other);

		Channel*			getChannelByName(const std::string& name) const;
		const t_channelsMap	&getChannels() const;
		
		Channel*			createChannel(const std::string& name, Client &creator);
		void				deleteChannel(const std::string& name);
		void				joinChannel(const std::string& channelName, const std::string& key, Client &client);
		void				removeClientFromAllChannels(Client &client);
};

#endif