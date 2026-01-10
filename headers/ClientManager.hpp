#ifndef CLIENT_MANAGER_HPP
#define CLIENT_MANAGER_HPP

#include <map>
#include <string>

class Client;
class Server;

typedef std::map<int, class Client> t_clientsMap;

class ClientManager
{
	private:
		t_clientsMap		_clients;
		
		ClientManager(const ClientManager& other);
		ClientManager &operator=(const ClientManager& other);
	public:
		~ClientManager();
		ClientManager();
		
		t_clientsMap&		getClients();
		const t_clientsMap&	getClients() const;

		void				addClient(Server &server, int socketFd);
		void				removeClient(int socketFd);
		bool				isExists(int socketFd) const;
		
		Client*				getClient(int socketFd);
		const Client*		getClient(int socketFd) const;
		Client*				getClientByNick(const std::string &name) const;
};

#endif