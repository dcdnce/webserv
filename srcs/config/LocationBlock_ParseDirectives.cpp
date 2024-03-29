#include "config/LocationBlock.hpp"
#include "utils/Logger.hpp"

#include <iostream>

LocationBlock::directiveFuncPtr LocationBlock::whichDirective(std::string const str)
{
	if (str == "root")
		return (&LocationBlock::parseDirective_root);
	if (str == "accepted_methods")
		return (&LocationBlock::parseDirective_acceptedMethods);
	if (str == "return")
		return (&LocationBlock::parseDirective_return);
	if (str == "autoindex")
		return (&LocationBlock::parseDirective_autoindex);
	if (str == "index")
		return (&LocationBlock::parseDirective_index);
	if (str == "upload_path")
		return (&LocationBlock::parseDirective_uploadedFilesPath);
	if (str == "cgi")
		return (&LocationBlock::parseDirective_cgi);

	Logger::warn(true) << "section \"location\": directive \"" + str + "\" not recognized: skipping" << std::endl;
	return (NULL);
}

void LocationBlock::parseDirective_root(std::string line)
{
	std::vector<std::string> params = _extractParams(line);

	if (params.size() != 1)
		throw std::runtime_error("directive \"root\" has wrong number of arguments");

	root = params[0];

	#ifdef DEBUG
	Logger::debug(true) << "LocationBlock::parseDirective_root: received line:" << line << std::endl;
	#endif
}

void LocationBlock::parseDirective_acceptedMethods(std::string line)
{
	std::vector<std::string> params = _extractParams(line);

	if (params.size() == 0)
		throw std::runtime_error("directive \"accepted_methods\" is missing arguments");

	acceptedMethods.clear();

	for (size_t i = 0; i < params.size(); i++)
	{
		if (http::methodsMap.find(params[i]) == http::methodsMap.end())
			Logger::warn(true) << "directive \"accepted_methods\": \"" << params[i] << "\" is not an HTTP Method: skipping" << std::endl;
		else
			acceptedMethods.insert(http::methodsMap[params[i]]);
	}

	#ifdef DEBUG
	Logger::debug(true) << "LocationBlock::parseDirective_acceptedMethods: received line:" << line << std::endl;
	#endif
}

void LocationBlock::parseDirective_return(std::string line)
{
	std::vector<std::string> params = _extractParams(line);

	if (params.size() != 2)
		throw std::runtime_error("directive \"return\" has wrong number of arguments");

	int httpcode;
	try
	{
		httpcode = std::stoi(params[0]);
	}
	catch (std::exception &e)
	{
		throw std::runtime_error("directive \"return\" error code not a number");
	}

	if (!http::isRedirection(httpcode))
	{
		throw std::runtime_error("directive \"return\" error code not recognized");
	}

	redirection = std::make_pair(static_cast<http::Status>(httpcode), params[1]);

	#ifdef DEBUG
	Logger::debug(true) << "LocationBlock::parseDirective_return: received line:" << line << std::endl;
	#endif
}

void LocationBlock::parseDirective_autoindex(std::string line)
{
	std::vector<std::string> params = _extractParams(line);

	if (params.size() != 1)
		throw std::runtime_error("directive \"autoindex\" has wrong number of arguments");

	if (params[0] != "on" && params[0] != "off")
	{
		throw std::runtime_error("directive \"autoindex\" argument not recognized");
	}

	autoindex = (params[0] == "on");

	#ifdef DEBUG
	Logger::debug(true) << "LocationBlock::parseDirective_autoindex: received line:" << line << std::endl;
	#endif
}

void LocationBlock::parseDirective_index(std::string line)
{
	indexes = _extractParams(line);

	if (!indexes.size())
		throw std::runtime_error("directive \"index\" is missing arguments");

	#ifdef DEBUG
	Logger::debug(true) << "LocationBlock::parseDirective_index: received line:" << line << std::endl;
	#endif
}

void LocationBlock::parseDirective_uploadedFilesPath(std::string line)
{
	std::vector<std::string> params = _extractParams(line);

	if (params.size() != 1)
		throw std::runtime_error("directive \"upload_path\" has wrong number of arguments");

	uploadPath = params[0];

	#ifdef DEBUG
	Logger::debug(true) << "LocationBlock::parseDirective_uploadedFilesPath: received line:" << line << std::endl;
	#endif
}

void LocationBlock::parseDirective_cgi(std::string line)
{
	std::vector<std::string> params = _extractParams(line);

	if (params.size() != 2)
		throw std::runtime_error("directive \"cgi\" has wrong number of arguments");

	if (!fs::exists(params[1]))
		throw std::runtime_error("directive \"cgi\": path \"" + params[1] + "\" does not exist");

	if (!fs::hasPermission(params[1], "x"))
		throw std::runtime_error("directive \"cgi\": path \"" + params[1] + "\" is not executable");

	this->cgis[params[0]] = params[1];

	#ifdef DEBUG
	Logger::debug(true) << "LocationBlock::parseDirective_cgi: received line:" << line << std::endl;
	#endif
}
