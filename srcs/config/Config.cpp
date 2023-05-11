#include "Config.hpp"

Config::Config(std::string const path) {
    _ifs.open(path);
    if (!_ifs.is_open()) {
        perror("Config::Config: abort: !_ifs.is_open()");
        throw std::runtime_error("Config::Config: abort: !_ifs.is_open()");
    }
}

Config::~Config() {
    _ifs.close();
}

void Config::_parsing() {
    static bool  alreadyParsed = 0;
    if (alreadyParsed) {
        perror("Config::parse: warning: alreadyParsed");
    }
    alreadyParsed = true;

    // parsing ...
}