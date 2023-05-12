#include "ServerBlock.hpp"
#include <iostream>

ServerBlock::ServerBlock() {

}

ServerBlock::~ServerBlock() {

}

void ServerBlock::setPortHost(std::string const str) {
    size_t i;

    for (i = 0 ; i < str.size() && str[i] != ':' ; i++);
    if (i == str.size())
        throw std::runtime_error("ServerBlock::setPortHost: abort: \"port:host\" not in the right format");

    _host = str.substr(0, i);
    try {
        _port = std::stoi(str.substr(i+1, str.size() - i)); 
    } catch (std::exception &e) {throw std::runtime_error("ServerBlock::setPortHost: abort: \"host\" not in the right format");};
}

ServerBlock::directiveFuncPtr ServerBlock::whichDirective(std::string const str) {
    if (str == "server_name")
        return (&ServerBlock::parseDirective_serverName);
    if (str == "error_page")
        return (&ServerBlock::parseDirective_errorPage);
    if (str == "client_max_body_size")
        return (&ServerBlock::parseDirective_clientMaxBodySize);

    std::string error = "ServerBlock::isDirective: warning: " + str + " is not an accepted directive for a server block: skipping";
    perror(error.c_str());

    return (nullptr);
}

void    ServerBlock::parseDirective_serverName(std::string line) {
    #ifdef DEBUG
    	Logger::debug(true) << "ServerBlock::parseDirective_serverName: received line:" << line << std::endl;
	#endif

    int i;
    _serverName = "";

    for (i = 0; std::isspace(line[i]) ; i++);
    for (; !std::isspace(line[i]) ; i++)
        _serverName += line[i];
}

void    ServerBlock::parseDirective_errorPage(std::string line) {
    #ifdef DEBUG
    	Logger::debug(true) << "ServerBlock::parseDirective_errorPage: received line:" << line << std::endl;
	#endif

}

void    ServerBlock::parseDirective_clientMaxBodySize(std::string line) {
    #ifdef DEBUG
    	Logger::debug(true) << "ServerBlock::parseDirective_clientMaxBodySize: received line:" << line << std::endl;
	#endif
}