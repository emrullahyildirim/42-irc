#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Server;

class Client
{
    private:
        int         _socketFd;
        bool        _isAuthenticated;
        std::string _username;  
        std::string _nickname;
        std::string _buffer;
        Server		*_server;

    public:
		~Client();
        Client(Server &server, int socketFd);
        Client(const Client& other);
        Client& operator=(const Client &other);

        int                getSocketFd() const;
        bool               getIsAuthenticated() const;
        const std::string& getUsername() const;
        const std::string& getNickname() const;
        const std::string& getSocketBuffer() const;
        Server&            getServer() const;

        void appendBuffer(const std::string& str);
        void eraseBuffer(size_t pos, size_t len);
        void authenticate(const std::string &username, const std::string &nickname);
};

#endif