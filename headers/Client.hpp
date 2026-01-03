#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Server;

class Client
{
	private:
		int _socketFd;
		bool _isAuthenticated;
		std::string _username;	
		std::string _nickname;
		Server& _server;
	public:
		Client(Server &server, int socketFd);
		Client& operator=(const Client &other);

		int getSocketFd() const;
		bool getIsAuthenticated() const;
		std::string getUsername() const;
		std::string getNickname() const;
		Server& getServer() const;

		void authenticate(const std::string &username, const std::string &nickname);
};

#endif