#ifndef CHANNELMANAGER_HPP
#define CHANNELMANAGER_HPP
#include <string>
#include <map>
#include "Channel.hpp"

class Client;
class Server;

class ChannelManager 
{
	private:
		std::map<std::string, Channel*>	_channels;
		Server* 						_server;
	public:
		~ChannelManager();
		ChannelManager(Server &server);
		ChannelManager(const ChannelManager& other);
		ChannelManager& operator=(const ChannelManager &other);

		Channel*	getChannelByName(const std::string& name);
		std::map<std::string, Channel*>&	getChannels();
		
		Channel*	createChannel(const std::string& name, Client &creator);
		void		deleteChannel(const std::string& name);
		void		joinChannel(const std::string& channelName, const std::string& key, Client &client);
		void		removeClientFromAllChannels(Client &client);
};

#endif