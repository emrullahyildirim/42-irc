#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include "../headers/Server.hpp"
#include "../headers/Client.hpp"

Server::~Server() {
    for (size_t i = 0; i < _socketPoolFds.size(); i++)
        close(_socketPoolFds[i].fd);
}

Server::Server(int serverPort, std::string serverPassword) : _port(serverPort), _password(serverPassword), _socketFd(-1) {}

Server::Server(const Server &other) { *this = other; }

Server &Server::operator=(const Server &other) {
    if (this != &other) {
        _port = other._port;
        _password = other._password;
        _socketFd = other._socketFd;
        _socketPoolFds = other._socketPoolFds;
    }
    return *this;
}

std::string Server::getPassword() const { return _password; }

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

	onClientConnect(newSocket);
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
		std::string clientBuffer = client->getSocketBuffer();
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
			client->eraseBuffer(0, pos + 1); 
			clientBuffer = client->getSocketBuffer();
		}
	}
	else
		onClientDisconnect(socketFd);
}

void Server::onClientConnect(int socketFd)
{
	_clientManager.addClient(*this, socketFd);
}

void Server::onClientDisconnect(int socketFd)
{
	for (std::vector<struct pollfd>::iterator it = _socketPoolFds.begin(); it != _socketPoolFds.end(); ++it)
        if (it->fd == socketFd) {
            _socketPoolFds.erase(it);
            break;
        }

	_clientManager.removeClient(socketFd);
	close(socketFd);
}

void Server::onClientMessage(Client &client, std::string data) {
    std::cout << data << std::endl;
}