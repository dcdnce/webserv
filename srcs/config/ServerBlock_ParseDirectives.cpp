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

    return (nullptr);
}

void    ServerBlock::parseDirective_serverName(std::string line) {
    for (size_t i = 0 ; i < line.size() ;) {
        std::string currWord = "";
        for (; std::isspace(line[i]) && i < line.size() ; i++);
        for (; !std::isspace(line[i]) && i < line.size() ; i++)
            currWord += line[i];
        if (currWord.size())
            _serverName.push_back(currWord);
    }
    
    if (_serverName.size() == 0)
        throw std::runtime_error("ServerBlock::parseDirective_serverName: abort: directive missing arguments");

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

    std::vector<std::string>    words;
    for (size_t i = 0 ; i < line.size() ;) {
        std::string currWord = "";
        for (; std::isspace(line[i]) && i < line.size() ; i++);
        for (; !std::isspace(line[i]) && i < line.size() ; i++)
            currWord += line[i];
        words.push_back(currWord);
    }

    if (words.size() < 2)
        throw std::runtime_error("ServerBlock::parseDirective_errorPage: abort: directive missing arguments");

    for (size_t i = 0 ; i < words.size() - 1 ; i++) {
        int httpcode;
        try {
            httpcode = std::stoi(words[i]);
        } catch (std::exception &e) {
            Logger::warn(true) << "ServerBlock::parseDirective_errorPage: error code not a number: skipping" << std::endl;
            continue ;
        };

        if ((httpcode > 511 || httpcode < 500)) {
            Logger::warn(true) << "ServerBlock::parseDirective_errorPage: error code not recognized: skipping" << std::endl;;
            continue ;
        }

       _errorPages[httpcode] = words[words.size()-1]; 
    }
}

void    ServerBlock::parseDirective_clientMaxBodySize(std::string line) {
    #ifdef DEBUG
    	Logger::debug(true) << "ServerBlock::parseDirective_clientMaxBodySize: received line:" << line << std::endl;
	#endif

    std::vector<std::string>    words;
    for (size_t i = 0 ; i < line.size() ;) {
        std::string currWord = "";
        for (; std::isspace(line[i]) && i < line.size() ; i++);
        for (; !std::isspace(line[i]) && i < line.size() ; i++)
            currWord += line[i];
        words.push_back(currWord);
    }

    if (words.size() != 1)
        throw std::runtime_error("ServerBlock::parseDirective_clientMaxBodySize: abort: directive wrong number of arguments");

    if (std::toupper(words[0][words[0].size()-1]) != 'M')
        throw std::runtime_error("ServerBlock::parseDirective_clientMaxBodySize: abort: directive argument not recognized");

}