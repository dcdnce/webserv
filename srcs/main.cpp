#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAX_CLIENTS	10
#define BUFFER_SIZE	1 << 8

class Logger
{
	private:
		static std::string	_getTime(void)
		{
			time_t		now = time(0);
			struct tm	tstruct;
			char		buf[80];

			tstruct = *localtime(&now);
			strftime(buf, sizeof(buf), "%X", &tstruct);
			return (buf);
		}

	public:
		static std::ostream&	info(const bool time = false)
		{
			if (time)
				std::cout << "\e[1;37;40m " << _getTime() << " \e[0m";
			std::cout << "\e[1;44m INFO \e[0m ";
			return (std::cout);
		}

		static std::ostream&	warn(const bool time = false)
		{
			if (time)
				std::cout << "\e[1;37;40m " << _getTime() << " \e[0m";
			std::cout << "\e[1;43m WARN \e[0m ";
			return (std::cout);
		}

		static std::ostream&	error(const bool time = false)
		{
			if (time)
				std::cout << "\e[1;37;40m " << _getTime() << " \e[0m";
			std::cout << "\e[1;41m ERROR \e[0m ";
			return (std::cout);
		}

		static std::ostream&	debug(const bool time = false)
		{
			if (time)
				std::cout << "\e[1;37;40m " << _getTime() << " \e[0m";
			std::cout << "\e[1;45m DEBUG \e[0m ";
			return (std::cout);
		}
};

namespace http
{

	class Client
	{
		private:
			int			_socket_fd;
			sockaddr_in	_addr;
			socklen_t	_addr_len;

		public:
			Client(void):
				_socket_fd(-1),
				_addr_len(sizeof(_addr))
			{}

			~Client(void)
			{
				if (_socket_fd != -1)
					::close(_socket_fd);
			}

			void	accept(const int serverSocket)
			{
				if ((_socket_fd = ::accept(serverSocket, (struct sockaddr*)&_addr, &_addr_len)) == -1)
					throw std::runtime_error("Client::accept: abort: accept()");
			}

			void	close(void)
			{
				if (_socket_fd != -1)
					::close(_socket_fd);
				_socket_fd = -1;
			}

			bool	occupied(void) const
			{
				return (_socket_fd != -1);
			}

			int		getSocket(void) const
			{
				return (_socket_fd);
			}

			sockaddr_in&	getAddr(void)
			{
				return (_addr);
			}

			socklen_t&	getAddrLen(void)
			{
				return (_addr_len);
			}

			friend std::ostream&	operator<<(std::ostream& os, const Client& client)
			{
				os << inet_ntoa(client._addr.sin_addr) << ":" << ntohs(client._addr.sin_port);
				return (os);
			}
	};

	class ClientManager
	{
		private:
			std::vector<Client>	_clients;
			int					_maxClients;

		public:
			ClientManager(void):
				_clients(MAX_CLIENTS),
				_maxClients(MAX_CLIENTS)
			{}

			~ClientManager(void)
			{}

			void	acceptConnection(const int serverSocket)
			{
				for (int i = 0; i < MAX_CLIENTS; i++)
					if (!_clients[i].occupied())
					{
						_clients[i].accept(serverSocket);
						return ;
					}
				Logger::warn(true) << "ClientManager::acceptConnection: abort: no available slot" << std::endl;
			}

			void	closeConnection(const int i)
			{
				_clients[i].close();
			}

			void	closeAllConnections(void)
			{
				for (int i = 0; i < MAX_CLIENTS; i++)
					_clients[i].close();
			}

			Client&	getClient(const int i)
			{
				return (_clients[i]);
			}

			int		getMaxClients(void) const
			{
				return (_maxClients);
			}

	};

	class Socket
	{
		protected:
			const char*	_host;
			int			_port;
			int			_socket;
			int			_domain;
			int			_type;
			int			_protocol;

		public:
			Socket(
				const int domain,
				const int type,
				const int protocol,
				const char* host,
				const int port
			):
				_host(host),
				_port(port),
				_socket(socket(domain, type, protocol)),
				_domain(domain),
				_type(type),
				_protocol(protocol)
			{
				const int	enable = 1;

				if (_socket == -1)
					throw std::runtime_error("Socket constructor: abort: socket()");

				// Set socket option to allow to rebind to an already used port
				// https://learn.microsoft.com/en-us/windows/win32/winsock/using-so-reuseaddr-and-so-exclusiveaddruse#using-so_reuseaddr
				if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
				{
					close(_socket);
					perror("Socket constructor: abort: setsockopt()");
					throw std::runtime_error("Socket constructor: abort: setsockopt()");
				}

				sockaddr_in	addr;

				memset(&addr, 0, sizeof(addr));
				addr.sin_family = domain;			// Internet protocol
				addr.sin_port = htons(port);		// Port to listen to
				addr.sin_addr.s_addr = INADDR_ANY;	// Listen to any address

				// Bind socket to address
				if (bind(_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)
				{
					close(_socket);
					perror("Socket constructor: abort: bind()");
					throw std::runtime_error("Socket constructor: abort: bind()");
				}
			}

			~Socket()
			{}

			void	listen(void)
			{
				if (::listen(_socket, MAX_CLIENTS) == -1)
					throw std::runtime_error("Socket::listen: abort: listen()");
				#ifdef DEBUG
					Logger::debug(true) << "Listening on port " << _port << std::endl;
				#endif
			}

			int		getSocket(void) const
			{
				return (_socket);
			}
	};

	class WebServer: public Socket
	{
		private:
			ClientManager	_clientManager;
			fd_set			_readfds;
			int				_maxfd;
			// int		_clients[MAX_CLIENTS];

			void	_reset(void)
			{
				FD_ZERO(&_readfds);
				FD_SET(_socket, &_readfds);
				_maxfd = _socket;

				for (int i = 0; i < _clientManager.getMaxClients(); i++)
				{
					Client&	client = _clientManager.getClient(i);

					if (client.occupied())
					{
						FD_SET(client.getSocket(), &_readfds);
						if (client.getSocket() > _maxfd)
							_maxfd = client.getSocket();
					}
				}
			}

		public:
			WebServer(
				const char *host,
				const int port
			):
				Socket(AF_INET, SOCK_STREAM, 0, host, port),
				_maxfd(_socket)
			{
				FD_ZERO(&_readfds);
			}

			~WebServer()
			{}

			void	listen(void)
			{
				int	connections = 0;

				// Start listening for incoming connections
				Socket::listen();

				#ifdef DEBUG
					Logger::debug(true) << "Waiting for incoming connections" << std::endl;
				#endif

				// Handle incoming connections in a loop
				while (true)
				{
					_reset();

					// Wait for activity on one of the sockets
					if ((connections = select(_maxfd + 1, &_readfds, NULL, NULL, NULL)) == -1)
					{
						perror("WebServer::listen: abort: select()");
						throw std::runtime_error("WebServer::listen: abort: select()");
					}

					if (connections == 0)
						continue;

					// Check if it's an incoming connection on the server socket
					if (FD_ISSET(_socket, &_readfds))
						_clientManager.acceptConnection(_socket);

					// Check if it's an incoming connection on one of the client sockets
					for (int i = 0; i < _clientManager.getMaxClients(); i++)
					{
						Client&	client = _clientManager.getClient(i);

						if (!client.occupied() || !FD_ISSET(client.getSocket(), &_readfds))
							continue;

						char	buffer[1024] = {0};
						int		bytes;

						// Read data from client
						if ((bytes = recv(client.getSocket(), buffer, sizeof(buffer), 0)) == -1)
						{
							perror("WebServer::listen: abort: recv()");
							throw std::runtime_error("WebServer::listen: abort: recv()");
						}

						if (bytes == 0)
						{
							// Client disconnected
							#ifdef DEBUG
								Logger::debug(true) << client << " disconnected" << std::endl;
							#endif
							client.close();
							continue;
						}

						// Print out the data received
						buffer[bytes] = '\0';
						#ifdef DEBUG
							Logger::debug(true) << client << " sent: " << buffer << std::endl;
						#endif

						const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>";

						if (send(client.getSocket(), response, strlen(response), 0) == -1)
						{
							perror("WebServer::listen: abort: send()");
							throw std::runtime_error("WebServer::listen: abort: send()");
						}

						// Close the connection
						client.close();
					}
				}
			}
	};

}

int	main(void)
{
	http::WebServer	server(NULL, 8080);

	server.listen();
	return (EXIT_SUCCESS);
}

int	__DONOTUSE(void)
{
	http::WebServer	server("10.12.10.3", 8080);
	fd_set			readfds;
	int				maxfd, connections, clientSockets[MAX_CLIENTS];

	for (int i = 0 ; i < MAX_CLIENTS ; i++)
		clientSockets[i] = -1;

	server.listen();

	// Handle incoming connections in a loop
	while (true)
	{
		FD_ZERO(&readfds);
		// Add the server socket to the set so we can listen to it
		FD_SET(server.getSocket(), &readfds);
		maxfd = server.getSocket();

		// Add the client sockets to the set so we can listen to them
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			if (clientSockets[i] > -1)
			{
				FD_SET(clientSockets[i], &readfds);
				if (clientSockets[i] > maxfd)
					maxfd = clientSockets[i];
			}
		}

		// select() will block until a FD is ready to be read, written or has an error
		if ((connections = select(maxfd + 1, &readfds, NULL, NULL, NULL)) == -1)
		{
			perror("select()");
			return (EXIT_FAILURE);
		}

		// If there is no connection, wait for new ones
		if (connections == 0)
			continue;

		// If the server socket woke up, it means there is a new connection
		if (FD_ISSET(server.getSocket(), &readfds))
		{
			int			clientSocket;
			sockaddr_in	clientAddr;
			socklen_t	clientAddrLen = sizeof(clientAddr);

			if ((clientSocket = accept(server.getSocket(), (struct sockaddr*)&clientAddr, &clientAddrLen)) == -1)
			{
				perror("accept()");
				return (EXIT_FAILURE);
			}

			#ifdef DEBUG
				Logger::debug(true) << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
			#endif

			// Add the new client socket to the set
			for (int i = 0; i < MAX_CLIENTS; i++)
				if (clientSockets[i] == -1)
				{
					clientSockets[i] = clientSocket;
					break;
				}
		}

		// If a client socket woke up, it means there is data to read
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			if (clientSockets[i] == -1 || !FD_ISSET(clientSockets[i], &readfds))
				continue;

			char	buffer[1024] = {0};
			int		bytes;

			if ((bytes = recv(clientSockets[i], buffer, sizeof(buffer), 0)) == -1)
			{
				perror("recv()");
				return (EXIT_FAILURE);
			}

			#ifdef DEBUG
				Logger::debug(true) << "Received " << bytes << " bytes from client " << i << std::endl;
			#endif

			if (bytes == 0)
			{
				#ifdef DEBUG
					Logger::debug(true) << "Client " << i << " disconnected" << std::endl;
				#endif
				close(clientSockets[i]);
				clientSockets[i] = -1;
				continue;
			}

			const char	*response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello World!</h1></body></html>\r\n";

			if (send(clientSockets[i], response, strlen(response), 0) == -1)
			{
				perror("send()");
				return (EXIT_FAILURE);
			}

			#ifdef DEBUG
				Logger::debug(true) << "Sent " << strlen(response) << " bytes to client " << i << std::endl;
			#endif

			close(clientSockets[i]);
			clientSockets[i] = -1;
		}
	}
	return (EXIT_SUCCESS);
}
