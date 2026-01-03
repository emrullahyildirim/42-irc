#ifndef CLIENT_MANAGER_HPP
#define CLIENT_MANAGER_HPP

#include <map>
#include "Client.hpp"

class ClientManager
{
	private:
		std::map<int, class Client> _clients;
	public:
		ClientManager();
		
		void addClient(const Client &client);
		void removeClient(int socketFd);
		Client* getClient(int socketFd);

		bool isClientExists(int socketFd) const;
};

#endif