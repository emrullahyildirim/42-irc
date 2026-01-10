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
	public:
		~ClientManager();
		ClientManager();
		ClientManager(const ClientManager& other);
		ClientManager &operator=(const ClientManager& other);
		
		t_clientsMap&		getClients();
		const t_clientsMap&	getClients() const;

		void				addClient(Server &server, int socketFd);
		void				removeClient(int socketFd);
		bool				isExists(int socketFd) const;
		
		Client*				getClient(int socketFd);
		const Client*		getClient(int socketFd) const;
		Client*				getClientByNick(std::string &name) const;
};

#endif