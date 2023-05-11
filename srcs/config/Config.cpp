#include "Config.hpp"
#include <iostream>

Config::Config(std::string const path) {
    _ifs.open(path);
    if (!_ifs.is_open()) {
        perror("Config::Config: abort: !_ifs.is_open()");
        throw std::runtime_error("Config::Config: abort: !_ifs.is_open()");
    }

    this->_parsing();
}

Config::~Config() {
    _ifs.close();
}

void Config::_parsing() {
    static bool  alreadyParsed = 0;
    if (alreadyParsed) {
        throw std::runtime_error("Config::parse: warning: alreadyParsed");
    }
    alreadyParsed = true;

    //while (!_ifs.eof())
    _parseServerBlock();
}

std::string Config::_getWord() {
    std::string word = "";

    for (; std::isspace(_ifs.peek()) ; _ifs.get());
    for (; !std::isspace(_ifs.peek()) && !_ifs.eof() ; word += _ifs.get());
    for (; std::isspace(_ifs.peek()) ; _ifs.get());

    return (word);
}