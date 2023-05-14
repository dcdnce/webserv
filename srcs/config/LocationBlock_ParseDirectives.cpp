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
   	    Logger::debug(true) << "ServerBlock::parseDirective_root: received line:" << line << std::endl;
    #endif
}

void    LocationBlock::parseDirective_acceptedMethods(std::string line) {
   	Logger::debug(true) << "ServerBlock::parseDirective_acceptedMethods: received line:" << line << std::endl;
}

void    LocationBlock::parseDirective_return(std::string line) {
   	Logger::debug(true) << "ServerBlock::parseDirective_return: received line:" << line << std::endl;
}

void    LocationBlock::parseDirective_autoindex(std::string line) {
    Logger::debug(true) << "ServerBlock::parseDirective_autoindex: received line:" << line << std::endl;
}

void    LocationBlock::parseDirective_index(std::string line) {
    Logger::debug(true) << "ServerBlock::parseDirective_index: received line:" << line << std::endl;
}

void    LocationBlock::parseDirective_uploadedFilesPath(std::string line) {
    Logger::debug(true) << "ServerBlock::parseDirective_uploadedFilesPath: received line:" << line << std::endl;
}

void    LocationBlock::parseDirective_cgi(std::string line) {
    Logger::debug(true) << "ServerBlock::parseDirective_uploadedFilescgi: received line:" << line << std::endl;
}