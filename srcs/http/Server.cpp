#include "http/Server.hpp"

namespace http
{

	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Server::Server(const ServerBlock &config):
		_config(config)
	{}

	Server::~Server(void)
	{}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	const ServerBlock &Server::getConfig(void) const { return (this->_config); }

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	bool Server::matches(const http::Client& client) const
	{
		const http::Host& host = client.getHost();
		ServerBlock::listensVector listens = _config.listens;

		// TODO: Check if the host matches the server name if it is set
		for (ServerBlock::listensVector::const_iterator it = listens.begin(); it != listens.end(); ++it)
			if (it->getPort() == host.getPort() && (it->getAddr().sin_addr.s_addr == INADDR_ANY || it->getAddr().sin_addr.s_addr == host.getAddr().sin_addr.s_addr))
					return (true);

		return (false);
	}

	http::Response	Server::handleRequest(const http::Client& client) const
	{
		const http::Request& request = client.getRequest();
		http::Response response;
		LocationBlock location;

		try
		{
			location = _config.locations.at(request.getUri());
		}
		catch (const std::out_of_range& e)
		{
			response.setStatus(NOT_FOUND);
			return response;
		}

		#ifdef DEBUG
		Logger::debug(true) << "Location found: " << location.uri << std::endl;
		#endif

		#ifdef DEBUG
		Logger::debug(true) << "Accepted methods: ";
		for (LocationBlock::methodsSet::const_iterator it = location.acceptedMethods.begin(); it != location.acceptedMethods.end(); ++it)
			std::cout << methodToStr(*it) << " ";
		std::cout << std::endl;
		Logger::debug(true) << "Request method: " << methodToStr(request.getMethod()) << std::endl;
		Logger::debug(true) << "Is accepted: " << (location.acceptedMethods.find(request.getMethod()) != location.acceptedMethods.end()) << std::endl;
		#endif

		if (location.acceptedMethods.size() > 0 && location.acceptedMethods.find(request.getMethod()) == location.acceptedMethods.end())
		{
			response.setStatus(METHOD_NOT_ALLOWED);
			return response;
		}

		return response;
	}

}
