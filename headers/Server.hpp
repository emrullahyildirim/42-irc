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

class Server
{
	private:
		const int					_port;
		const std::string			_password;
		int							_socketFd;
		std::vector<struct pollfd>	_socketPoolFds;

		void handlePollEvents(struct pollfd&);
		void handleNewConnection();
		void handleNewData(int);
		void onClientConnect();
		void onClientDisconnect();
		void onClientData(char *);
	public:
		~Server();
		Server();
		Server(int, std::string);
		Server(const Server &);
		Server&	operator=(const Server &);

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