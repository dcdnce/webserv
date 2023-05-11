#include "Config.hpp"

void Config::_parseServerBlock() {
    ServerBlock    currServer;

    _parseServerBlockHeader(currServer);
    //_parseServerBlockBody(currServer);
}

void Config::_parseServerBlockHeader(ServerBlock & currServer) {
    if (_getWord() != "server")
        throw std::runtime_error("Config::_parseServerBlock: abort: \"server\" block not found");
    
    currServer.setPortHost(_getWord());

    if (_getWord() != "{")
        throw std::runtime_error("Config::_parseServerBlock: abort: \"server\" block opening bracket error");

}