#include <cstddef> 
#include "../headers/ClientManager.hpp"
#include "../headers/Client.hpp"

ClientManager::~ClientManager() {}

ClientManager::ClientManager() {}

ClientManager::ClientManager(const ClientManager& other) {
	*this = other;
}

ClientManager &ClientManager::operator=(const ClientManager& other)
{
	if (this != &other)
		this->_clients = other._clients;
	return (*this);
}

void ClientManager::addClient(Server &server, int socketFd) {
    if (_clients.find(socketFd) == _clients.end())
        _clients.insert(std::make_pair(socketFd, Client(server, socketFd)));
}

void ClientManager::removeClient(int socketFd)
{
	_clients.erase(socketFd);
}

bool ClientManager::isExists(int socketFd) const
{
	return (_clients.find(socketFd) != _clients.end());
}

Client* ClientManager::getClient(int socketFd) {
    std::map<int, Client>::iterator it = _clients.find(socketFd);

    if (it == _clients.end()) 
		return NULL;

    return &(it->second);
}

const Client* ClientManager::getClient(int socketFd) const {
    std::map<int, Client>::const_iterator it = _clients.find(socketFd);

    if (it == _clients.end())
		return NULL;
		
    return &(it->second);
}

std::map<int, class Client> &ClientManager::getClients()
{
	return _clients;
}

const std::map<int, class Client> &ClientManager::getClients() const
{
	return _clients;
}

