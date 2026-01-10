#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Server;

class Client
{
    private:
        int					_socketFd;
        bool				_isAuthenticated;
		bool				_isRegistered;
        std::string			_username;
		std::string			_realname;  
        std::string			_nickname;
		std::string			_hostname;
        std::string			_buffer;
        Server*				_server;

    public:
		~Client();
        Client(Server &server, int socketFd);
        Client(const Client& other);
        Client& operator=(const Client &other);

        int               	getSocketFd() const;
        bool				getIsAuthenticated() const;
		bool				getIsRegistered() const;
        Server&       		getServer() const;
        const std::string&	getUsername() const;
        const std::string&	getNickname() const;
		const std::string&	getHostname() const;
		
		void				setAuthenticated(bool authenticated);
		void 				setNickname(const std::string &nickname);
		void 				setUsername(const std::string &username);
		void 				setRealname(const std::string &realname);
		void 				setHostname(const std::string &hostname);
		
		void				sendMessage(const std::string &message);
		
        void				appendBuffer(const std::string& str);
        void				eraseBuffer(size_t pos, size_t len);
        const std::string&	getBuffer() const;

		
        void				checkRegistration();
		std::string			getPrefix() const;
		static bool			isValidNickname(const std::string& nick);

};

#endif