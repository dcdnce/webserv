#include "config/LocationBlock.hpp"
#include "utils/Logger.hpp"

#include <iostream>

LocationBlock::directiveFuncPtr    LocationBlock::whichDirective(std::string const str) {
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
    if (str == "uploaded_files_path")
        return (&LocationBlock::parseDirective_uploadedFilesPath);
    if (str == "cgi")
        return (&LocationBlock::parseDirective_cgi);

    Logger::warn(true) << "LocationBlock::isDirective: \"" + str + "\" is not an accepted directive for a location block: skipping" << std::endl;

    return (NULL);
}

void    LocationBlock::parseDirective_root(std::string line) {
    std::vector<std::string> params = _extractParams(line);

    if (params.size() != 1)
        throw std::runtime_error("LocationBlock::parseDirective_root: abort: wrong number of arguments for directive");

    _root = params[0];

    #ifdef DEBUG
   	    Logger::debug(true) << "LocationBlock::parseDirective_root: received line:" << line << std::endl;
    #endif
}

void    LocationBlock::parseDirective_acceptedMethods(std::string line) {
    std::vector<std::string>    params = _extractParams(line);

    if (params.size() == 0)
        throw std::runtime_error("LocationBlock::parseDirective_acceptedMethods: abort: missing arguments for directive");

    for (size_t i = 0 ; i < params.size() ; i++) {
        if (http::methodsMap.find(params[i]) == http::methodsMap.end())
            Logger::warn(true) << "LocationBlock::parseDirective_acceptedMethods: warning: \"" << params[i] << "\" is not an HTTP Method: skipping" << std::endl;
        else
            _acceptedMethods.insert(http::methodsMap[params[i]]);
    }

    #ifdef DEBUG
   	    Logger::debug(true) << "LocationBlock::parseDirective_acceptedMethods: received line:" << line << std::endl; 
    #endif
}

void    LocationBlock::parseDirective_return(std::string line) {
    std::vector<std::string>    params = _extractParams(line);

    if (params.size() != 2)
        throw std::runtime_error("LocationBlock::parseDirective_return: abort: wrong number of arguments for directive");

    int httpcode;
    try {
        httpcode = std::stoi(params[0]);
    } catch (std::exception &e) {
        Logger::warn(true) << "LocationBlock::parseDirective_return: error code not a number: skipping" << std::endl;
        return ;
    }

    if (!http::isRedirection(httpcode)) {
        Logger::warn(true) << "LocationBlock::parseDirective_return: error code not recognized: skipping" << std::endl;;
        return ;
    }

    _redirections[httpcode] = params[params.size()-1]; 

    #ifdef DEBUG
   	    Logger::debug(true) << "LocationBlock::parseDirective_return: received line:" << line << std::endl;
    #endif
}

void    LocationBlock::parseDirective_autoindex(std::string line) {
    std::vector<std::string>    params = _extractParams(line);

    if (params.size() != 1)
        throw std::runtime_error("LocationBlock::parseDirective_autoindex: abort: wrong number of arguments for directive");

    if (params[0] != "on" && params[0] != "off") {
        Logger::warn(true) << "LocationBlock::parseDirective_autoindex: argument not recognized: skipping" << std::endl;;
        return ;
    }

   _autoindex = (params[0] == "on");
 
    #ifdef DEBUG
        Logger::debug(true) << "LocationBlock::parseDirective_autoindex: received line:" << line << std::endl;
    #endif
}

void    LocationBlock::parseDirective_index(std::string line) {
    _indexes = _extractParams(line);

    if (!_indexes.size())
        throw std::runtime_error("LocationBlock::parseDirective_index: abort: missing arguments for directive");

    #ifdef DEBUG
        Logger::debug(true) << "ServerBlock::parseDirective_index: received line:" << line << std::endl;
    #endif
}

void    LocationBlock::parseDirective_uploadedFilesPath(std::string line) {
    std::vector<std::string>    params = _extractParams(line);

    if (params.size() != 1)
        throw std::runtime_error("LocationBlock::parseDirective_uploadedFilesPath: abort: wrong number of arguments for directive");

    _uploadPath = params[0];

    #ifdef DEBUG
        Logger::debug(true) << "ServerBlock::parseDirective_uploadedFilesPath: received line:" << line << std::endl;
    #endif
}

void    LocationBlock::parseDirective_cgi(std::string line) {
    Logger::debug(true) << "ServerBlock::parseDirective_cgi: received line:" << line << std::endl;
}