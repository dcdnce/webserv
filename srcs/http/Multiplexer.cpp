#include "http/Multiplexer.hpp"

namespace http
{

	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Multiplexer::Multiplexer(const Config& config):
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

		for (std::vector<ServerBlock>::iterator sbIt = serverBlocks.begin(); sbIt != serverBlocks.end(); sbIt++)
		{
			_servers.push_back(new Server(*sbIt));

			for (ServerBlock::listensVector::const_iterator listenIt = sbIt->listens.begin(); listenIt != sbIt->listens.end(); listenIt++)
				ports.insert(listenIt->getPort());
		}

		// For each unique port, create a socket
		for (std::set<int>::iterator it = ports.begin(); it != ports.end(); it++)
		{
			http::Socket	*socket = new http::Socket(AF_INET, SOCK_STREAM, 0, NULL, *it);

			Logger::info(true) << "Creating socket [" << socket->getSocket() << "] on port " << *it << std::endl;
			_sockets.push_back(socket);
		}
	}

	Multiplexer::~Multiplexer(void)
	{}

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void	Multiplexer::reset(void)
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

	void	Multiplexer::listen(void)
	{
		int	ret = 0;

		// Setup sockets to listen
		for (socket_list::iterator it = _sockets.begin(); it != _sockets.end(); it++)
			(*it)->listen();

		Logger::info(true) << "Listening for connections..." << std::endl;

		while (true)
		{
			// Reset all
			reset();

			// Wait for an event
			if ((ret = select(_maxfd + 1, &_readfds, &_writefds, NULL, NULL)) == -1)
			{
				perror("select");
				exit(EXIT_FAILURE);
			}

			// Handle new connections to sockets
			for (socket_list::iterator it = _sockets.begin(); it != _sockets.end(); it++) {
				if (FD_ISSET((*it)->getSocket(), &_readfds)) {
					_clientManager.acceptConnection(**it);
				}
			}

			// Handle client requests
			for (int i = 0; i < _clientManager.getMaxClients(); i++)
			{
				http::Client&	client = _clientManager.getClient(i);

				if (!client.isOccupied())
					continue;

				if (FD_ISSET(client.getSocket(), &_readfds))
				{
					// Receive request headers
					if (!client.headerReceived)
					{
						try
						{
							client.receive();
						}
						catch(const http::Client::ClientDisconnectedException& e)
						{
							#ifdef DEBUG
							Logger::debug(true) << "Client [" << client << "] disconnected" << std::endl;
							#endif

							client.close();
							continue;
						}
						catch(const std::exception& e)
						{
							#ifdef DEBUG
							Logger::debug(true) << "Client [" << client << "] error" << std::endl;
							std::cout << e.what() << std::endl;
							#endif

							client.close();
							continue;
						}

						if (client.getRawRequest().find("\r\n\r\n") == std::string::npos)
							continue;

						client.parseRequest();
						client.headerReceived = true;
					}

					// Receive request body
					if (client.headerReceived)
					{
						if (client.getRequest().hasHeader("Content-Length"))
						{
							int contentLength = std::stoi(client.getRequest().getHeaders().at("Content-Length"));

							if (contentLength > 0)
							{
								try
								{
									client.receive();
								}
								catch(const http::Client::ClientDisconnectedException& e)
								{
									#ifdef DEBUG
									Logger::debug(true) << "Client [" << client << "] disconnected" << std::endl;
									#endif

									client.close();
									continue;
								}
								catch(const std::exception& e)
								{
									#ifdef DEBUG
									Logger::debug(true) << "Client [" << client << "] error: " << strerror(errno) << std::endl;
									#endif

									client.close();
									continue;
								}

								if (((int)client.getRawRequest().size() - (int)client.getRawRequest().find("\r\n\r\n") - 4) < contentLength)
									continue;
							}
						}

						client.parseRequest();

						#ifdef DEBUG
						Logger::debug(true) << "================================================" << std::endl;
						Logger::debug(true) << "\e[1D\e[1;97;44m RECV \e[0m "
							<< methodToStr(client.getRequest().getMethod()) << " "
							<< client.getRequest().getUrl().raw << " "
							<< client.getRequest().getHttpVersion() << std::endl;
						#endif

						// Client is ready to be processed
						FD_SET(client.getSocket(), &_writefds);
					}
				}
				if (FD_ISSET(client.getSocket(), &_writefds))
				{
					Server *server = NULL;
					int	send_ret = 1;

					// Find the server to handle the request
					for (server_list::iterator sit = _servers.begin(); sit != _servers.end(); sit++)
						if ((*sit)->matches(client))
						{
							server = *sit;
							break;
						}

					if (server != NULL)
					{
						http::Response response = server->handleRequest(client);

						#ifdef DEBUG
						const double bytes = response.getBody().size();
						std::string humanReadableBytes;

						if (bytes < 1024)
							humanReadableBytes = std::to_string(bytes) + " B";
						else if (bytes < 1024 * 1024)
							humanReadableBytes = std::to_string(bytes / 1024) + " KB";
						else if (bytes < 1024 * 1024 * 1024)
							humanReadableBytes = std::to_string(bytes / (1024 * 1024)) + " MB";
						else
							humanReadableBytes = std::to_string(bytes / (1024 * 1024 * 1024)) + " GB";

						Logger::debug(true) << "\e[1D\e[1;97;42m SEND \e[0m "
							<< response.getStatus() << " ("
							<< humanReadableBytes << ")" << std::endl;
						#endif

						send_ret = client.send(response);
					}
					else
					{
						// No server found, send a 404
						http::Response response;
						response.setStatus(NOT_FOUND);
						send_ret = client.send(response);
					}

					FD_CLR(client.getSocket(), &_writefds);
					if (send_ret == 0)
					{
						#ifdef DEBUG
						Logger::debug(true) << "\e[1D\e[1;97;41m CLSD \e[0m " << client << std::endl;
						#endif
						client.close();
					}
					else
					{
						client.clear();
					}
				}
			}
		}
	}

}
