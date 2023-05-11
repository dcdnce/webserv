#include "ServerBlock.hpp"
#include <iostream>

ServerBlock::ServerBlock() {

}

ServerBlock::~ServerBlock() {

}

void ServerBlock::setPortHost(std::string const word) {
    size_t i;

    for (i = 0 ; i < word.size() && word[i] != ':' ; i++);
    if (i == word.size())
        throw std::runtime_error("ServerBlock::setPortHost: abort: \"port:host\" not in the right format");

    _host = word.substr(0, i);
    try {
        _port = std::stoi(word.substr(i+1, word.size() - i)); 
    } catch (std::exception &e) {throw std::runtime_error("ServerBlock::setPortHost: abort: \"host\" not in the right format");};
}