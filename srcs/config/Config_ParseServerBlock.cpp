#include "Config.hpp"

void Config::_parseServerBlock() {
    ServerBlock    currServer;

    _parseServerBlockHeader(currServer);
    _parseServerBlockBody(currServer);
    //check if mandatory directives are here
}

void Config::_parseServerBlockHeader(ServerBlock & currServer) {
    if (_getWord() != "server")
        throw std::runtime_error("Config::_parseServerBlockHeader: abort: \"server\" block not found");
    
    currServer.setPortHost(_getWord());

    if (_getWord() != "{")
        throw std::runtime_error("Config::_parseServerBlockHeader: abort: \"server\" block opening bracket error");
}

void Config::_parseServerBlockBody(ServerBlock & currServer) {
    std::string currWord;

     for (; currWord != "}" ; currWord = _getWord()) {
        if (_ifs.eof())
            throw std::runtime_error("Config::_parseServerBlockBody: abort: \"server\" block ending bracket error");
        
        //if == location

        if (!currServer.isDirective(currWord)) {
            std::string error = "ServerBlock::isDirective: warning: " + currWord + " is not an accepted directive for a server block: skipping";
            perror(error.c_str());
            _skipToSemicolon();
            continue ;
        }
    }
}