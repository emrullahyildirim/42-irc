#include <cstddef> 
#include "../headers/ClientManager.hpp"
#include "../headers/Client.hpp"

ClientManager::~ClientManager() {}

ClientManager::ClientManager() {}

void ClientManager::addClient(Server &server, int socketFd) {
    if (_clients.find(socketFd) == _clients.end())
        _clients.insert(std::make_pair(socketFd, Client(server, socketFd)));
}

void ClientManager::removeClient(int socketFd) {
	
	_clients.erase(socketFd);
}

bool ClientManager::isExists(int socketFd) const {
	return (_clients.find(socketFd) != _clients.end());
}

Client* ClientManager::getClient(int socketFd) {
    t_clientsMap::iterator it = _clients.find(socketFd);

    if (it == _clients.end()) 
		return (NULL);

    return &(it->second);
}

const Client* ClientManager::getClient(int socketFd) const {
    t_clientsMap::const_iterator it = _clients.find(socketFd);

    if (it == _clients.end())
		return (NULL);
		
    return &(it->second);
}

Client* ClientManager::getClientByNick(const std::string &name) const {
	for (t_clientsMap::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
		if (it->second.getNickname() == name) 
			return const_cast<Client*>(&(it->second));
	return (NULL);
}

std::map<int, class Client> &ClientManager::getClients()
{
	return (_clients);
}

const std::map<int, class Client> &ClientManager::getClients() const
{
	return (_clients);
}

