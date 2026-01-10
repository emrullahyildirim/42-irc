#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>

class Client;

typedef std::vector<Client *> t_clientVector;
typedef std::map<std::string, bool> t_inviteMap;

class Channel 
{
	private:
		std::string				_name;
		std::string				_topic;
		std::string				_password;
		size_t					_limit;
		bool					_inviteOnly;
		bool					_topicRestricted;
		
		t_clientVector			_clients;
		t_clientVector			_operators;
		t_inviteMap				_invited;

		Channel(const Channel& other);
		Channel& operator=(const Channel &other);
	public:
		~Channel();
		Channel(const std::string &name);

		const std::string 		&getName() const;
		const std::string 		&getPassword() const;
		const std::string 		&getTopic() const;
		const t_clientVector	&getClients() const;
		size_t					getLimit() const;
		bool					isInviteOnly() const;
		bool					isTopicRestricted() const;

		void					setName(const std::string& name);
		void					setPassword(const std::string& password);	
		void 					setTopic(const std::string& topic);
		void					setLimit(size_t limit);
		void					setInviteOnly(bool value);
		void					setTopicRestricted(bool value);

		void 					addClient(Client* client);
		void 					removeClient(Client* client);
		bool					isClientInChannel(Client* client) const;
		size_t					getClientCount() const;
		
		bool					isOperator(Client* client) const;
		void					addOperator(Client* client);
		void					removeOperator(Client* client);

		bool					isInvited(const std::string& nickname) const;
		void					addInvite(const std::string& nickname);
		void					removeInvite(const std::string& nickname);
		
		void 					broadcast(const std::string& message, Client* except = NULL);
		std::string				getClientListString() const;
		std::string				getModeString() const;
};

#endif