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

bool ServerBlock::isDirective(std::string const str) {
    return (str == "server_name" || str == "error_page" || str == "client_max_body_size");
}