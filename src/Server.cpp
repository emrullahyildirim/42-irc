#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include <sstream>
#include "../headers/Server.hpp"
#include "../headers/Parser.hpp"
#include "../headers/Client.hpp"
#include "../headers/Commands.hpp"
#include "../headers/Command.hpp"

Server::~Server() {
    for (size_t i = 0; i < _socketPoolFds.size(); i++)
        close(_socketPoolFds[i].fd);
}

Server::Server(std::string name, int serverPort, std::string serverPassword) 
: _name(name), _port(serverPort), _password(serverPassword), _socketFd(-1), _channelManager(*this) {
	initializeCommands();
}

Server::Server(const Server &other) : _channelManager(*this) { 
	*this = other; 
	initializeCommands();
}

Server &Server::operator=(const Server &other) {
    if (this != &other) {
		_name = other._name;
        _port = other._port;
        _password = other._password;
        _socketFd = other._socketFd;
        _socketPoolFds = other._socketPoolFds;
    }
    return *this;
}

int	Server::getPort() const { return (_port); }
const std::string &Server::getPassword() const { return (_password); }
const std::string &Server::getName() const { return (_name); }
ChannelManager &Server::getChannelManager() { return (_channelManager); }
ClientManager &Server::getClientManager() { return (_clientManager); }
CommandManager &Server::getCommandManager() { return (_commandManager); }

void Server::Initialize() {
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);

	int status = 1;
	if (_socketFd == -1 
		|| setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &status, sizeof(status)) == -1 
		|| fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Couldn't initialize socket connection!");

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);

	if (bind(_socketFd, (struct sockaddr *)&address, sizeof(address)) == -1)
		throw std::runtime_error("Couldn't bind socket to the port!");
	if (listen(_socketFd, 3) == -1)
		throw std::runtime_error("Couldn't listen on the socket!");

	struct pollfd server_pfd;
	server_pfd.fd = _socketFd;
	server_pfd.events = POLLIN;
	_socketPoolFds.push_back(server_pfd);
}

void Server::Run()
{
	std::cout << "Server is running on port " << _port << std::endl;
	while (true)
	{
		int ret = poll(&_socketPoolFds[0], _socketPoolFds.size(), -1);
		if (ret < 0)
			throw std::runtime_error("Poll error occurred!");

		for (size_t i = 0; i < _socketPoolFds.size(); i++)
		{
			if (!(_socketPoolFds[i].revents & POLLIN))
				continue;

			if (_socketPoolFds[i].fd == _socketFd)
				handleNewConnection();
			else
				handleNewData(_socketPoolFds[i].fd);
		}
	}
}

void Server::handleNewConnection()
{
	struct sockaddr_in clientAddr;
	socklen_t addrLen = sizeof(clientAddr);
	int newSocket = accept(_socketFd, (struct sockaddr *)&clientAddr, &addrLen);

	if (newSocket == -1)
		return ;	
	fcntl(newSocket, F_SETFL, O_NONBLOCK);
	struct pollfd client_pfd;
	client_pfd.fd = newSocket;
	client_pfd.events = POLLIN;
	_socketPoolFds.push_back(client_pfd);

	_clientManager.addClient(*this, newSocket);
	Client *client = _clientManager.getClient(newSocket);
	client->setHostname(inet_ntoa(clientAddr.sin_addr));
}

void Server::handleNewData(int socketFd)
{
	char buffer[1024] = {0};
	int valread = read(socketFd, buffer, 1024);
	if (valread > 0)
	{
		Client *client = _clientManager.getClient(socketFd);
        if (!client) return;

		client->appendBuffer(std::string(buffer, valread));
		std::string clientBuffer = client->getBuffer();
		while (1)
		{
			size_t pos = clientBuffer.find("\n");
			if (pos == std::string::npos)
				break;
			
			std::string	msg = clientBuffer.substr(0, pos);
			
			if (!msg.empty())
			{
				int	msgLen = msg.size();
				if (msg[msgLen - 1] == '\r')
				msg.erase(msgLen - 1);
				onClientMessage(*client, msg);
			}
			client = _clientManager.getClient(socketFd);
			if (!client)
				return;
			client->eraseBuffer(0, pos + 1); 
			clientBuffer = client->getBuffer();
		}
	}
	else if (valread == 0)
		onClientDisconnect(socketFd);
}


void Server::onClientDisconnect(int socketFd)
{
	Client *client = _clientManager.getClient(socketFd);
	if (client) {
		_channelManager.removeClientFromAllChannels(*client);
	}
	
	for (std::vector<struct pollfd>::iterator it = _socketPoolFds.begin(); it != _socketPoolFds.end(); ++it)
        if (it->fd == socketFd) {
            _socketPoolFds.erase(it);
            break;
        }
	_clientManager.removeClient(socketFd);
	close(socketFd);
}

void Server::onClientMessage(Client &client, std::string data) {
	std::cout << "Gelen: " << data << std::endl;
	Parser parsedCmd(data);
	
	_commandManager.executeCommand(client, parsedCmd);
}

void Server::reply(Client &client, int code, const std::string &msg) const {
    std::stringstream ss;
	if (code < 10)
		ss << "00";
	else if (code < 100)
		ss << '0';
    ss << code;
    std::string codeStr = ss.str();
    
    std::string fullMsg = ":" + _name + " " + codeStr + " " + client.getNickname() + " " + msg;
    client.sendMessage(fullMsg);
}

void Server::initializeCommands()
{
	_commandManager.registerCommand(Command("CAP", "CAP COMMAND", Command_Cap));
	_commandManager.registerCommand(Command("PING", "PING COMMAND", Command_Ping));
	_commandManager.registerCommand(Command("PASS", "PASS COMMAND", Command_Pass));
	_commandManager.registerCommand(Command("NICK", "NICK COMMAND", Command_Nick));
	_commandManager.registerCommand(Command("USER", "USER COMMAND", Command_User));
	_commandManager.registerCommand(Command("JOIN", "JOIN COMMAND", Command_Join));
	_commandManager.registerCommand(Command("PRIVMSG", "PRIVMSG COMMAND", Command_Privmsg));
}

void Server::disconnectClient(Client &client)
{
	int	fd = client.getSocketFd();
	_channelManager.removeClientFromAllChannels(client);
	close(fd);
	for (size_t i = 0; i < _socketPoolFds.size(); ++i) 
        if (_socketPoolFds[i].fd == fd) {
            _socketPoolFds.erase(_socketPoolFds.begin() + i);
            break;
        }
	_clientManager.removeClient(fd);
}

bool Server::isNickInUse(const std::string &nick) const
{
    const std::map<int, Client> &clients = _clientManager.getClients();
    
    std::map<int, Client>::const_iterator it;
    for (it = clients.begin(); it != clients.end(); ++it)
        if (it->second.getNickname() == nick)
            return true;
    return false;
}