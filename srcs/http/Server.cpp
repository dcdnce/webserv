#include "http/Server.hpp"
#include "utils/FileSystem.hpp"
#include "cgi/Cgi.hpp"

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

			// Check if the request uri matches the location uri as a prefix
			if (request.getUrl().path.compare(0, uri.size(), uri) == 0)
				if (location == NULL || location->uri.size() < uri.size())
					location = &(it->second);
		}

		if (location == NULL)
		{
			response.setStatus(NOT_FOUND);
			return (response);
		}

		// Handle autoindex
		if (location->autoindex && !location->root.empty())
		{
			const std::string path = request.getUrl().path;
			const std::string filePath = fs::joinPaths(location->root, path);

			if (fs::isDir(filePath))
			{
				const std::vector<std::string> files = fs::readDir(filePath);

				response.setStatus(OK);
				response.setBody("<html><body><h1>Index of " + path + "</h1><hr><ul>");
				for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
					response.appendToBody("<li><a href=\"" + fs::joinPaths(path, *it) + "\">" + *it + "</a></li>");
				response.appendToBody("</ul><hr></body></html>");
			}
			else if (fs::exists(filePath) && fs::hasPermission(filePath, "r"))
				response.setBody(fs::readFile(filePath));
			return (response);
		}

		// Handle index
		if (location->indexes.size() > 0 && !location->root.empty())
		{
			response.setStatus(OK);

			// Send the first index that exists
			for (LocationBlock::indexesVector::const_iterator it = location->indexes.begin(); it != location->indexes.end(); it++)
			{
				const std::string filePath = fs::joinPaths(location->root, *it);

				if (fs::exists(filePath) && access(filePath.c_str(), R_OK) == 0)
				{
					response.setBody(fs::readFile(filePath));
					break;
				}
			}

			return (response);
		}

		// Handle CGIs
		if (location->cgis.size() > 0 && !location->root.empty())
		{
			const std::string extension = fs::getExtension(request.getUrl().path);

			#ifdef DEBUG
			Logger::debug(true) << "Extension: " << extension << std::endl;
			#endif

			if (location->cgis.find(extension) == location->cgis.end())
			{
				response.setStatus(NOT_FOUND);
				return (response);
			}

			#ifdef DEBUG
			Logger::debug(true) << "CGI found" << std::endl;
			#endif

			const Cgi &cgi = location->cgis.at(extension);

			// Check if the CGI exists and is executable
			if (!fs::exists(cgi.getPath()) || !fs::hasPermission(cgi.getPath(), "x"))
			{
				response.setStatus(NOT_FOUND);
				return (response);
			}

			const std::string filePath = fs::joinPaths(location->root, request.getUrl().path);

			switch (request.getMethod())
			{
				case GET:
					response.fromCGI(cgi.executeGet(filePath, request));
					break;
				case POST:
					response.fromCGI(cgi.executePost(filePath, request));
					break;
				default:
					response.setStatus(NOT_IMPLEMENTED);
					return (response);
			}

			return (response);
		}

		return (response);
	}

}
