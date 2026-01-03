#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <poll.h>
#include <exception>
#include "ClientManager.hpp" 

class Server
{
    private:
        int                         _port;
        std::string                 _password;
        int                         _socketFd;
        std::vector<struct pollfd>  _socketPoolFds;
        ClientManager               _clientManager;

        void handlePollEvents(struct pollfd& pollfd);
        void handleNewConnection();
        void handleNewData(int fd);
        void onClientConnect(int socketFd);
        void onClientDisconnect(int socketFd);
        void onClientMessage(Client &client, std::string data);
        void parseCommand(const std::string &message, std::string &command, std::string &args);

    public:
        Server(int port, std::string password);
        Server(const Server &other);
        Server& operator=(const Server &other);
        ~Server();

        std::string getPassword() const;

        void Initialize();
        void Run();
};

#endif