#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include "config/ServerBlock.hpp"
#include "utils/Logger.hpp"
#include "http/http.hpp"

ServerBlock::directiveFuncPtr ServerBlock::whichDirective(std::string const str)
{
	if (str == "server_name")
		return (&ServerBlock::parseDirective_serverName);
	if (str == "error_page")
		return (&ServerBlock::parseDirective_errorPage);
	if (str == "client_max_body_size")
		return (&ServerBlock::parseDirective_clientMaxBodySize);
	if (str == "listen")
		return (&ServerBlock::parseDirective_listen);

	Logger::warn(true) << "section \"server\": directive \"" + str + "\" not recognized: skipping" << std::endl;

	return (NULL);
}

void ServerBlock::parseDirective_listen(std::string line)
{
	std::vector<std::string> params = _extractParams(line);

	if (params.size() != 1)
		throw std::runtime_error("directive \"listen\" has wrong number of arguments");

	std::string arg = params[0];
	std::string host = "0.0.0.0";
	std::string port = "80";
	std::string::size_type colonPos = arg.find(':');

	if (colonPos != std::string::npos)
	{
		host = arg.substr(0, colonPos);
		port = arg.substr(colonPos + 1);

		if (host.empty() || host.compare(0, 1, "*") == 0)
			host = "0.0.0.0";

		if (std::isdigit(host.front()) && !http::isValidIP(host))
			throw std::runtime_error("directive \"listen\" has invalid host");

		if (!http::isValidPort(port))
			throw std::runtime_error("directive \"listen\" has invalid port");
	}
	else
	{
		if (std::isdigit(arg.front()) && http::isValidIP(arg))
			host = arg;
		else if (http::isValidPort(arg))
			port = arg;
		else
			throw std::runtime_error("directive \"listen\" has invalid host or port");
	}

	std::vector<http::Host> hosts = http::Host::getHosts(host, port);

	if (hosts.size() == 0)
		throw std::runtime_error("directive \"listen\" has no available interface");

	listens.push_back(hosts.front());

	#ifdef DEBUG
	Logger::debug(true) << "ServerBlock::parseDirective_host: received line:" << line << std::endl;
	#endif
}

void ServerBlock::parseDirective_serverName(std::string line)
{
	serverNames = _extractParams(line);

	if (serverNames.size() == 0)
		throw std::runtime_error("directive \"server_name\" is missing arguments");

#ifdef DEBUG
	Logger::debug(true);
	for (size_t i = 0; i < serverNames.size(); i++)
		std::cout << serverNames[i] << " ";
	std::cout << std::endl;
#endif
}

void ServerBlock::parseDirective_errorPage(std::string line)
{
#ifdef DEBUG
	Logger::debug(true) << "ServerBlock::parseDirective_errorPage: received line:" << line << std::endl;
#endif

	std::vector<std::string> params = _extractParams(line);

	if (params.size() < 2)
		throw std::runtime_error("directive \"error_page\" is missing arguments");

	for (size_t i = 0; i < params.size() - 1; i++)
	{
		int httpcode;
		try
		{
			httpcode = std::stoi(params[i]);
		}
		catch (std::exception &e)
		{
			Logger::warn(true) << "directive \"error_page\" error code is not a number: skipping" << std::endl;
			continue;
		};

		if (!(http::isErrorClient(httpcode)) && !(http::isErrorServer(httpcode)))
		{
			Logger::warn(true) << "directive \"error_page\" error code is not recognized: skipping" << std::endl;
			;
			continue;
		}

		errorPages[httpcode] = params[params.size() - 1];
	}
}

void ServerBlock::parseDirective_clientMaxBodySize(std::string line)
{
	#ifdef DEBUG
	Logger::debug(true) << "ServerBlock::parseDirective_clientMaxBodySize: received line:" << line << std::endl;
	#endif

	std::vector<std::string> params = _extractParams(line);

	if (params.size() != 1)
		throw std::runtime_error("directive client_max_body_size has wrong number of arguments");

	if (params[0].back() != 'M')
		throw std::runtime_error("directive client_max_body_size argument is not recognized");

	params[0].pop_back();

	try
	{
		maxBodySize = std::stoull(params[0]) * MEGABYTE;
		#ifdef DEBUG
		Logger::debug(true) << "ServerBlock::parseDirective_clientMaxBodySize: maxBodySize set to " << maxBodySize << std::endl;
		#endif
	}
	catch (std::exception &e) { throw std::runtime_error("directive client_max_body_size argument is not a number"); }
}
