#include "config/ServerBlock.hpp"

#include "utils/Logger.hpp"

#include <iostream>
#include <sstream>
#include <vector>

//TODO -> defaults options in constructor

ServerBlock::ServerBlock() {
}

ServerBlock::~ServerBlock() {
}

std::string    ServerBlock::_getWord(std::string line, size_t from) {
    std::string word = "";

    for (; std::isspace(line[from]) && from < line.size() ; from++);
    for (; !std::isspace(line[from]) && from < line.size() ; from++)
        word += line[from];

    return (word);
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

    size_t i;
    _serverName = "";

    for (i = 0; std::isspace(line[i]) && i < line.size() ; i++);
    for (; !std::isspace(line[i]) && i < line.size() ; i++)
        _serverName += line[i];
}

void    ServerBlock::parseDirective_errorPage(std::string line) {
    #ifdef DEBUG
    	Logger::debug(true) << "ServerBlock::parseDirective_errorPage: received line:" << line << std::endl;
	#endif

    std::vector<std::string>    words;

    for (size_t i = 0 ; i < line.size() ;) {
        std::string currWord = "";
        for (; std::isspace(line[i]) && i < line.size() ; i++);
        for (; !std::isspace(line[i]) && i < line.size() ; i++)
            currWord += line[i];
        words.push_back(currWord);
    }

    if (words.size() < 2)
        throw std::runtime_error("ServerBlock::parseDirective_errorPage: abort: \"error_page\" directive missing arguments");

    for (size_t i = 0 ; i < words.size() - 1 ; i++) {
        int httpcode;
        try {
            httpcode = std::stoi(words[i]);
        } catch (std::exception &e) {throw std::runtime_error("ServerBlock::parseDirective_errorPage: abort: \"error_code\" not in the right format");};
        if (httpcode < 100 ||)
       _errorPages[words[i]] = words[words.size()-1]; 
    }

    // for (size_t j = 0 ; j < words.size() ; j++)
    //     std::cout << words[j] << std::endl;
}

void    ServerBlock::parseDirective_clientMaxBodySize(std::string line) {
    #ifdef DEBUG
    	Logger::debug(true) << "ServerBlock::parseDirective_clientMaxBodySize: received line:" << line << std::endl;
	#endif

    (void)line;
}

std::string ServerBlock::getPortHost() {
    return (_host + ":" + std::to_string(_port));
}