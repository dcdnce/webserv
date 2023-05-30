#include "http/Server.hpp"

#include "utils/FileSystem.hpp"

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
		const LocationBlock *location = NULL;

		for (ServerBlock::locationsMap::const_iterator it = _config.locations.begin(); it != _config.locations.end(); ++it)
		{
			const std::string& uri = it->first;

			Logger::info(true) << "Checking " << uri << std::endl;
			// Check if the request uri matches the location uri as a prefix
			if (request.getUri().compare(0, uri.size(), uri) == 0)
				if (location == NULL || location->uri.size() < uri.size())
				{
					Logger::info(true) << "Switching to location " << uri << std::endl;
					location = &(it->second);
				}
		}

		if (location == NULL)
		{
			response.setStatus(NOT_FOUND);
			return (response);
		}

		// Handle autoindex
		if (location->autoindex && !location->root.empty())
		{
			std::vector<std::string> files = fs::readDir(fs::joinPaths(location->root, request.getUri()));

			response.setStatus(OK);
			response.setBody("<html><body><h1>Index of " + request.getUri() + "</h1><hr><ul>");
			for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
				response.appendToBody("<li><a href=\"" + fs::joinPaths(request.getUri(), *it) + "\">" + *it + "</a></li>");
			response.appendToBody("</ul><hr></body></html>");
			return (response);
		}

		return response;
	}

}
