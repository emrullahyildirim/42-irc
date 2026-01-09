#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include "Server.hpp"

class Client
{
    private:
        int         _socketFd;
        bool        _isAuthenticated;
		bool		_isRegistered;
        std::string _username;
		std::string	_realname;  
        std::string _nickname;
		std::string	_hostname;
        std::string _buffer;
        Server		*_server;

    public:
		~Client();
        Client(Server &server, int socketFd);
        Client(const Client& other);
        Client& operator=(const Client &other);

        int               	getSocketFd() const;
        bool				getIsAuthenticated() const;
		bool				getIsRegistered() const;
        const std::string&	getUsername() const;
        const std::string&	getNickname() const;
		const std::string&	getHostname() const;
        const std::string&	getSocketBuffer() const;
        Server&           	getServer() const;

		void setAuthenticated(bool authenticated);
		void setNickname(std::string nickname);
		void setUsername(std::string username);
		void setRealname(std::string realname);
		void setHostname(std::string hostname);

		void sendMessage(std::string buffer);	
        void appendBuffer(const std::string& str);
        void eraseBuffer(size_t pos, size_t len);
        void CheckRegisteration();

		static bool isValidNickname(const std::string& nick);

};

#endif