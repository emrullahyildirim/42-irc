#ifndef CLIENT_MANAGER_HPP
#define CLIENT_MANAGER_HPP

#include <map>
#include <string>

class Client;
class Server;

class ClientManager
{
	private:
		std::map<int, class Client> _clients;
	public:
		~ClientManager();
		ClientManager();
		ClientManager(const ClientManager& other);
		ClientManager &operator=(const ClientManager& other);
		
		void addClient(Server &server, int socketFd);
		void removeClient(int socketFd);
		bool isExists(int socketFd) const;
		
		Client	*getClient(int socketFd);
		Client  *getClientByNick(std::string &name) const;
		const Client *getClient(int socketFd) const;
		std::map<int, class Client> &getClients();
		const std::map<int, class Client> &getClients() const;
};

#endif