#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>

class Client;

class Channel 
{
	private:
		std::string             _name;
		std::string             _topic;
		std::string             _password;
		size_t                  _limit;
		
		std::vector<Client*>    _clients;
		std::vector<Client*>    _operators;
	public:
		~Channel();
		Channel(std::string name);
		Channel(const Channel& other);
		Channel& operator=(const Channel &other);

		std::string getName() const;
		std::string getPassword() const;
		std::string getTopic() const;
		std::vector<Client*>& getClients();


		size_t getClintCount() const;

		void setTopic(const std::string& topic);
		void addClient(Client* client);
		void removeClient(Client* client);
		void broadcast(const std::string& message, Client* except = NULL);
		
		bool isOperator(Client* client) const;
		void addOperator(Client* client);
		void removeOperator(Client* client);
		bool isClientInChannel(Client* client) const;
		std::string getClientListString() const;
};

#endif