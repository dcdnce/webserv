#include "http/Multiplexer.hpp"

namespace http
{

	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Multiplexer::Multiplexer(const Config& config):
		_servers(),
		_sockets(),
		_clients(),
		_readfds(),
		_writefds(),
		_maxfd(0)
	{
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
			http::Socket	*socket = NULL;

			try { socket = new http::Socket(AF_INET, SOCK_STREAM, 0, NULL, *it); }
			catch (const std::exception& e)
			{
				Logger::error(true) << "Failed to create socket on port " << *it << ": " << e.what() << std::endl;
				continue;
			}

			#ifdef DEBUG
			Logger::debug(true) << "New socket(" << socket->getSocket() << ") listening on \e[1;32m" << *it << "\e[0m" << std::endl;
			#endif
			_sockets.push_back(socket);
		}
	}

	Multiplexer::~Multiplexer(void)
	{
		for (socket_list::iterator it = _sockets.begin(); it != _sockets.end(); it++)
			delete *it;

		for (server_list::iterator it = _servers.begin(); it != _servers.end(); it++)
			delete *it;
	}

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void	Multiplexer::initSelect(void)
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
		for (client_list::const_iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			const http::Client *client = *it;

			if (!client->requestComplete)
				FD_SET(client->getSocketFd(), &_readfds);
			else
				FD_SET(client->getSocketFd(), &_writefds);

			_maxfd = std::max(_maxfd, client->getSocketFd());
		}
	}

	void	Multiplexer::listen(void)
	{
		struct timeval timeout = { .tv_sec = 1, .tv_usec = 0 };
		int ret = 0;

		for (socket_list::iterator it = _sockets.begin(); it != _sockets.end(); it++)
			(*it)->listen();

		Logger::info(true) << "Listening on " << _sockets.size() << " socket(s)" << std::endl;

		while (true)
		{
			this->initSelect();

			if ((ret = select(_maxfd + 1, &_readfds, &_writefds, NULL, &timeout)) == -1)
			{
				Logger::error(true) << "select() failed: " << strerror(errno) << std::endl;
				continue;
			}

			if (ret == 0)
			{
				for (client_list::iterator it = _clients.begin(); it != _clients.end(); it++)
					if ((*it)->hasTimedOut())
					{
						#ifdef DEBUG
						Logger::debug(true) << "Client " << **it << " timed out" << std::endl;
						#endif
						delete *it;
						_clients.erase(it--);
					}
				continue;
			}

			// --- HANDLE CLIENTS --- //
			for (client_list::iterator it = _clients.begin(); it != _clients.end(); it++)
			{
				http::Client *client = *it;

				// --- READ --- //
				if (FD_ISSET(client->getSocketFd(), &_readfds))
				{
					try { client->receive(); }
					catch (const std::exception& e)
					{
						Logger::error(true) << "Failed to read from client: " << e.what() << std::endl;
						delete client;
						_clients.erase(it--);
						continue;
					}

					#ifdef DEBUG
					if (client->requestComplete)
					{
						Logger::debug(true) << *client << " Request received" << std::endl;
						Logger::block("Request", client->request.toString());
					}
					#endif
				}

				// --- WRITE --- //
				if (FD_ISSET(client->getSocketFd(), &_writefds))
				{
					if (!client->sending)
					{
						http::Server *server = NULL;

						#ifdef DEBUG
						Logger::debug(true) << *client << " Processing request..." << std::endl;
						#endif

						for (server_list::const_iterator it = _servers.begin(); it != _servers.end(); it++)
							if ((*it)->matches(*client))
							{
								server = *it;
								break;
							}

						if (server != NULL)
						{
							try { server->processRequest(*client); }
							catch (const std::exception& e)
							{
								client->response.setStatus(INTERNAL_SERVER_ERROR);
								client->response.setBody(e.what());
							}
						}
						else
						{
							client->response.setStatus(NOT_FOUND);
							client->response.setBody("No server found for this request");
						}

						client->sending = true;
					}

					try { client->send(); }
					catch (const std::exception& e)
					{
						delete client;
						_clients.erase(it--);
						continue;
					}

					if (client->responseSent())
					{
						#ifdef DEBUG
						Logger::debug(true) << *client << " Response sent (" << client->response.getBody().size() << " bytes)" << std::endl;
						#endif

						if (client->shouldClose())
						{
							#ifdef DEBUG
							Logger::debug(true) << *client << " Closing connection" << std::endl;
							#endif
							delete client;
							_clients.erase(it--);
							continue;
						}
						client->reset();
					}
				}

				if (client->hasTimedOut())
				{
					#ifdef DEBUG
					Logger::debug(true) << *client << " Timed out" << std::endl;
					#endif
					delete client;
					_clients.erase(it--);
					continue;
				}
			}

			// --- ACCEPT CLIENTS --- //
			for (socket_list::const_iterator it = _sockets.begin(); it != _sockets.end(); it++)
			{
				if (!FD_ISSET((*it)->getSocket(), &_readfds))
					continue;

				try
				{
					http::Client *client = new http::Client(**it);
					_clients.push_back(client);
					#ifdef DEBUG
					Logger::debug(true) << "Accepted client(" << _clients.back()->getSocketFd() << ")" << std::endl;
					#endif
				}
				catch (const std::exception& e) { Logger::error(true) << "Failed to accept client: " << e.what() << std::endl; }
			}
		}

	}

}
