#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <poll.h>
#include <exception>
#include "ClientManager.hpp" 
#include "CommandManager.hpp"

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

		void initializeCommands();
        void handlePollEvents(struct pollfd& pollfd);
        void handleNewConnection();
        void handleNewData(int fd);
        void onClientConnect(int socketFd);
        void onClientDisconnect(int socketFd);
        void onClientMessage(Client &client, std::string data);
        void parseCommand(const std::string &message, std::string &command, std::string &args);
		
	public:
        Server(std::string name, int port, std::string password);
        Server(const Server &other);
        Server& operator=(const Server &other);
        ~Server();
		
        const std::string &getPassword() const;
		const std::string &getName() const; 
		
        void Initialize();
        void Run();
		void reply(Client &client, int code, const std::string msg);
};

#endif