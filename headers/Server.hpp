#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <stdbool.h>
#include "ClientManager.hpp"

class Server
{
	private:
		const int					_port;
		const std::string			_password;
		int							_socketFd;
		std::vector<struct pollfd>	_socketPoolFds;
		ClientManager				_clientManager;

		void handlePollEvents(struct pollfd&);
		void handleNewConnection();
		void handleNewData(int);
		void onClientConnect(int socketFd);
		void onClientDisconnect(int socketFd);
		void onClientMessage(int socketFd, char *data);
		void parseCommand(const std::string &message, std::string &command, std::string &args);
	public:
		~Server();
		Server();
		Server(int, std::string);
		Server(const Server &);
		Server&	operator=(const Server &);

		std::string	getPassword() const;

		int	Initialize();
		int	Run();

		class RuntimeException : public std::exception
		{
			private:
				std::string	_message;
			public:
				RuntimeException(std::string);
				virtual ~RuntimeException() throw() {};
				const char* what() const throw();
		};

};

#endif