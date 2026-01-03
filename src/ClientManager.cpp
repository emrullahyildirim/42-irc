#include <cstddef> 
#include "../headers/ClientManager.hpp"

ClientManager::ClientManager() {}

void ClientManager::addClient(const Client &client)
{
	_clients.insert(std::pair<int, Client>(client.getSocketFd(), client));
}

void ClientManager::removeClient(int socketFd)
{
	_clients.erase(socketFd);
}

Client* ClientManager::getClient(int socketFd)
{
	std::map<int, Client>::iterator it = _clients.find(socketFd);
	if (it == _clients.end())
		return (NULL);
	return (&(it->second));
}

bool ClientManager::isClientExists(int socketFd) const
{
	return (_clients.find(socketFd) != _clients.end());
}

