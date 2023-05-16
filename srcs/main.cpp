#include "config/Config.hpp"
#include "http/Socket.hpp"
#include "http/ClientManager.hpp"

#include <set>

namespace test
{

	class Server
	{

		private:
			// -------------------------------------------------------------- //
			//  Attributes                                                    //
			// -------------------------------------------------------------- //
			ServerBlock	_config;

		public:
			// -------------------------------------------------------------- //
			//  Constructors & Destructors                                    //
			// -------------------------------------------------------------- //
			Server(const ServerBlock &config):
				_config(config)
			{
				Logger::info(true) << "Server created" << std::endl;
			}

			~Server(void)
			{
			}

	};

	class Multiplexer
	{
		public:
			// -------------------------------------------------------------- //
			//  Types                                                         //
			// -------------------------------------------------------------- //
			typedef std::vector<test::Server*>	server_list;
			typedef std::vector<http::Socket*>	socket_list;

		private:
			// -------------------------------------------------------------- //
			//  Attributes                                                    //
			// -------------------------------------------------------------- //
			server_list			_servers;
			socket_list			_sockets;
			http::ClientManager	_clientManager;

			fd_set	_readfds;
			fd_set	_writefds;
			int		_maxfd;

		public:
			// -------------------------------------------------------------- //
			//  Constructors & Destructors                                    //
			// -------------------------------------------------------------- //
			Multiplexer(const Config& config):
				_servers(),
				_sockets(),
				_clientManager(100),
				_readfds(),
				_writefds(),
				_maxfd(0)
			{
				this->reset();

				std::vector<ServerBlock> serverBlocks = config.getServerBlocks();
				std::set<int> ports;

				for (std::vector<ServerBlock>::iterator it = serverBlocks.begin(); it != serverBlocks.end(); it++)
				{
					_servers.push_back(new Server(*it));

					std::vector<std::pair<std::string, int> > listens = it->getListen();

					for (std::vector<std::pair<std::string, int> >::iterator it = listens.begin(); it != listens.end(); it++)
						ports.insert(it->second);
				}

				// For each unique port, create a socket
				for (std::set<int>::iterator it = ports.begin(); it != ports.end(); it++)
				{
					http::Socket	*socket = new http::Socket(AF_INET, SOCK_STREAM, 0, NULL, *it);

					Logger::info(true) << "Creating socket [" << socket->getSocket() << "] on port " << *it << std::endl;
					_sockets.push_back(socket);
				}
			}

			~Multiplexer(void)
			{
			}

			// -------------------------------------------------------------- //
			//  Public Methods                                                //
			// -------------------------------------------------------------- //
			void	reset(void)
			{
				// Clear the fd sets and reset the maxfd
				FD_ZERO(&_readfds);
				FD_ZERO(&_writefds);
				_maxfd = 0;

				// Add the server sockets to the fd sets
				for (socket_list::iterator it = _sockets.begin(); it != _sockets.end(); it++)
				{
					FD_SET((*it)->getSocket(), &_readfds);
					_maxfd = std::max(_maxfd, (*it)->getSocket());
				}

				// Add the client sockets to the fd sets
				for (int i = 0; i < _clientManager.getMaxClients(); i++)
				{
					const http::Client&	client = _clientManager.getClient(i);

					if (client.getSocket() != -1)
					{
						FD_SET(client.getSocket(), &_readfds);
						_maxfd = std::max(_maxfd, client.getSocket());
					}
				}
			}

			void	listen(void)
			{
				int	ret = 0;

				// Setup sockets to listen
				for (socket_list::iterator it = _sockets.begin(); it != _sockets.end(); it++)
					(*it)->listen();

				while (true)
				{
					// Reset all
					reset();

					#ifdef DEBUG
					Logger::debug(true) << "Maxfd: " << _maxfd << std::endl;
					Logger::debug(true) << "Waiting for connections..." << std::endl;
					#endif

					// Wait for an event
					if ((ret = select(_maxfd + 1, &_readfds, &_writefds, NULL, NULL)) == -1)
					{
						perror("select");
						exit(EXIT_FAILURE);
					}

					// Handle new connections to sockets
					for (socket_list::iterator it = _sockets.begin(); it != _sockets.end(); it++)
						if (FD_ISSET((*it)->getSocket(), &_readfds))
						{
							_clientManager.acceptConnection((*it)->getSocket());
							#ifdef DEBUG
							Logger::debug(true) << "New connection on socket [" << (*it)->getSocket() << "]" << std::endl;
							#endif
						}

					// Handle client requests
					for (int i = 0; i < _clientManager.getMaxClients(); i++)
					{
						http::Client&	client = _clientManager.getClient(i);

						if (!client.isOccupied())
							continue;

						if (FD_ISSET(client.getSocket(), &_readfds))
						{
							char	buffer[1024];
							int		bytesRead = 0;

							#ifdef DEBUG
							Logger::debug(true) << "New request on socket [" << client.getSocket() << "]" << std::endl;
							#endif

							if ((bytesRead = recv(client.getSocket(), buffer, sizeof(buffer) - 1, 0)) <= 0)
							{
								#ifdef DEBUG
								Logger::debug(true) << "Client [" << client.getSocket() << "] disconnected" << std::endl;
								#endif
								client.close();
								continue;
							}

							buffer[bytesRead] = '\0';
							Logger::info(true) << "Received " << bytesRead << " bytes from client [" << client << "]" << std::endl;
							Logger::info(true) << "Request: " << std::endl << buffer << std::endl;

							// TODO: Parse the request
						}
					}

				}
			}

	};

}

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: " << av[0] << " <config_file>" << std::endl;
		return (EXIT_FAILURE);
	}

	try
	{
		Config config(av[1]);
		test::Multiplexer multiplexer(config);

		multiplexer.listen();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
