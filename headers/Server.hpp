#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <string>
#include <poll.h>
#include <exception>

#include "ClientManager.hpp"
#include "CommandManager.hpp"
#include "ChannelManager.hpp"

class Server
{
    private:
		std::string					_name;
        int                         _port;
        std::string                 _password;

        int                         _socketFd;
        std::vector<struct pollfd>  _socketPoolFds;

        ClientManager               _clientManager;
		CommandManager				_commandManager;
		ChannelManager				_channelManager;

		void initializeCommands();
        void handlePollEvents(struct pollfd& pollfd);
        void handleNewConnection();
        void handleNewData(int fd);
        void onClientDisconnect(int socketFd);
        void onClientMessage(Client &client, std::string data);
        void parseCommand(const std::string &message, std::string &command, std::string &args);
		
	public:
		~Server();
        Server(std::string name, int port, std::string password);
        Server(const Server &other);
        Server& operator=(const Server &other);
		
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
	};

#endif