#include "config/ServerBlock.hpp"

#include "utils/Logger.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

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

std::string ServerBlock::getPortHost() const {
    return (_host + ":" + std::to_string(_port));
}

void    ServerBlock::addLocationBlock(LocationBlock newLocationBlock) {
    // replaced location block is now in the infinitesimal void of memory lost forever
    _locationBlocks[newLocationBlock.getPath()] = newLocationBlock;
}

std::vector<std::string> ServerBlock::_extractParams(std::string const & line) {
    std::vector<std::string>    params;

    for (size_t i = 0 ; i < line.size() ;) {
        std::string currWord = "";
        for (; std::isspace(line[i]) && i < line.size() ; i++);
        for (; !std::isspace(line[i]) && i < line.size() ; i++)
            currWord += line[i];
        if (currWord.size())
            params.push_back(currWord);
    }

    return (params);
}