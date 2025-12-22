#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstddef>

class Server
{
	private:
		static Server* _instance;
	public:
		Server();
		Server(const Server &obj) = delete; 
		static Server&	getInstance();
};

#endif