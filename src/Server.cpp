#include "../headers/Server.hpp"

Server::~Server() {}

Server::Server() : _port(6667), _password("defaultPassword") {}

Server::Server(int serverPort, std::string serverPassword) : _port(serverPort), _password(serverPassword) {}

Server &Server::operator=(const Server &other) {
	(void)other; 
	return *this;
}

std::string	Server::getPassword() const
{
	return (_password);
}

int Server::Initialize()
{
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	int status = 0;
	if (_socketFd == -1 || setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &status, sizeof(status)) == -1 || fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
		throw Server::RuntimeException("Couldn't initialize socket connection!");

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);

	if (bind(_socketFd, (struct sockaddr *)&address, sizeof(address)) == -1)
		throw Server::RuntimeException("Couldn't bind socket to the port!");
	if (listen(_socketFd, 3) == -1)
		throw Server::RuntimeException("Couldn't listen on the socket!");

	struct pollfd server_pfd;
	server_pfd.fd = _socketFd;
	server_pfd.events = POLLIN;
	_socketPoolFds.push_back(server_pfd);
	return (1);
}

int Server::Run()
{
	std::cout << "Server is running on port " << _port << std::endl;
	while (true)
	{
		int ret = poll(&_socketPoolFds[0], _socketPoolFds.size(), -1);
		if (ret < 0)
			throw Server::RuntimeException("Poll error occurred!");

		for (size_t i = 0; i < _socketPoolFds.size(); i++)
			handlePollEvents(_socketPoolFds[i]);
	}
	return (1);
}

void Server::handlePollEvents(struct pollfd &pfd)
{
	if (!(pfd.revents & POLLIN))
		return;
	if (pfd.fd == _socketFd)
		handleNewConnection();
	else
		handleNewData(pfd.fd);
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
		onClientMessage(socketFd, buffer);
	else
	{
		close(socketFd);
		onClientDisconnect(socketFd);
	}
}

void Server::onClientConnect(int socketFd)
{
	Client newClient = Client(*this, socketFd);
	_clientManager.addClient(newClient);
}

void Server::onClientDisconnect(int socketFd)
{
	_clientManager.removeClient(socketFd);
}

void Server::onClientMessage(int socketFd, char *data)
{
	Client* client = _clientManager.getClient(socketFd);
	if (!client)
		return ;

	std::cout << data << std::endl;
}

Server::RuntimeException::RuntimeException(std::string err) : _message(err)
{
}

const char *Server::RuntimeException::what() const throw()
{
	return _message.c_str();
}
