#include "config/ServerBlock.hpp"

#include "utils/Logger.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

//TODO 
//  - defaults options in constructor

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


std::string ServerBlock::getPortHost() {
    return (_host + ":" + std::to_string(_port));
}