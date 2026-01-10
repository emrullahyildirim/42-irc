#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <poll.h>

#include "ClientManager.hpp"
#include "CommandManager.hpp"
#include "ChannelManager.hpp"

class Server
{
    private:
		static bool					_shutdown;

		std::string					_name;
        int                         _port;
        std::string                 _password;

        int                         _socketFd;
        std::vector<struct pollfd>  _socketPoolFds;

        ClientManager               _clientManager;
		CommandManager				_commandManager;
		ChannelManager				_channelManager;

		void initializeCommands();
        void handleNewConnection();
        void handleNewData(int fd);
        void onClientDisconnect(int socketFd);
        void onClientMessage(Client &client, std::string data);
		
        Server(const Server &other);
        Server& operator=(const Server &other);
	public:
		~Server();
        Server(std::string name, int port, std::string password);

		int							getPort() const;
        const std::string&			getPassword() const;
		const std::string&			getName() const; 

		ChannelManager 				&getChannelManager();
		ClientManager 				&getClientManager();
		CommandManager 				&getCommandManager();	
		ChannelManager 				&getChannelManager() const;
		ClientManager 				&getClientManager() const;
		CommandManager 				&getCommandManager() const;		
		
        void 						Initialize();
        void 						Run();
		
		void 						reply(Client &client, int code, const std::string &msg) const;
		void 						disconnectClient(Client &client);
		
		bool						isNickInUse(const std::string &nick) const;

		static void					signalHandler(int signum);
	};

#endif