#include "config/ServerBlock.hpp"

#include "utils/Logger.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

ServerBlock::directiveFuncPtr ServerBlock::whichDirective(std::string const str) {
    if (str == "server_name")
        return (&ServerBlock::parseDirective_serverName);
    if (str == "error_page")
        return (&ServerBlock::parseDirective_errorPage);
    if (str == "client_max_body_size")
        return (&ServerBlock::parseDirective_clientMaxBodySize);

    Logger::warn(true) << "ServerBlock::isDirective: \"" + str + "\" is not an accepted directive for a server block: skipping" << std::endl;

    return (NULL);
}

void    ServerBlock::parseDirective_serverName(std::string line) {
    _serverName = _extractParams(line);
    
    if (_serverName.size() == 0)
        throw std::runtime_error("ServerBlock::parseDirective_serverName: abort: missing arguments for directive");

    #ifdef DEBUG
        Logger::debug();
        for (size_t i = 0 ; i < _serverName.size(); i++)
            std::cout << _serverName[i] << " ";
        std::cout << std::endl;
    #endif
}

void    ServerBlock::parseDirective_errorPage(std::string line) {
    #ifdef DEBUG
    	Logger::debug(true) << "ServerBlock::parseDirective_errorPage: received line:" << line << std::endl;
	#endif

    std::vector<std::string>    params = _extractParams(line);

    if (params.size() < 2)
        throw std::runtime_error("ServerBlock::parseDirective_errorPage: abort: missing arguments for directive");

    for (size_t i = 0 ; i < params.size() - 1 ; i++) {
        int httpcode;
        try {
            httpcode = std::stoi(params[i]);
        } catch (std::exception &e) {
            Logger::warn(true) << "ServerBlock::parseDirective_errorPage: error code not a number: skipping" << std::endl;
            continue ;
        };

        if (!(http::isErrorClient(httpcode)) && !(http::isErrorServer(httpcode))) {
            Logger::warn(true) << "ServerBlock::parseDirective_errorPage: error code not recognized: skipping" << std::endl;;
            continue ;
        }

       _errorPages[httpcode] = params[params.size()-1]; 
    }
}

void    ServerBlock::parseDirective_clientMaxBodySize(std::string line) {
    #ifdef DEBUG
    	Logger::debug(true) << "ServerBlock::parseDirective_clientMaxBodySize: received line:" << line << std::endl;
	#endif

    std::vector<std::string>    params = _extractParams(line);

    if (params.size() != 1)
        throw std::runtime_error("ServerBlock::parseDirective_clientMaxBodySize: abort: wrong number of arguments for directive");

    if (std::toupper(params[0][params[0].size()-1]) != 'M')
        throw std::runtime_error("ServerBlock::parseDirective_clientMaxBodySize: abort: argument not recognized for directive");
}