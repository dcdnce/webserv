#include "config/Config.hpp"

#include "utils/Logger.hpp"

#include <iostream>

void Config::_parseServerBlock() {
    ServerBlock    currServer;

    _parseServerBlockHeader(currServer);
    _parseServerBlockBody(currServer);
    
    _addServerBlock(currServer);
}

void Config::_parseServerBlockHeader(ServerBlock & currServer) {
    std::string currWord = _getWord();

    if (currWord != "server") {
        throw std::runtime_error("Config::_parseServerBlockHeader: abort: unknown block found");
    }
    
    currServer.setPortHost(_getWord());

    if (_getWord() != "{")
        throw std::runtime_error("Config::_parseServerBlockHeader: abort: block opening bracket error");

    #ifdef DEBUG
    	Logger::debug(true) << "Config::_parseServerBlockHeader: header read: " << "server " << currServer.getPortHost() << std::endl;
	#endif
}

void Config::_parseServerBlockBody(ServerBlock & currServer) {
    std::string currWord = " ";
    std::string currLine;

    while (true) {
        currWord = _getWord();

        // End of server block
        if (currWord == "}")
            return ;

        // No ending bracket
        if (_ifs.eof())
            throw std::runtime_error("Config::_parseServerBlockBody: abort: \"server\" block ending bracket error");
        
        // Found location block
        if (currWord == "location") {
            _parseLocationBlock(currServer);
            continue ;
        }

        // Found directive
        ServerBlock::directiveFuncPtr currDirective = currServer.whichDirective(currWord);
        if (currDirective == NULL) {
            _skipToNewline();
            continue ;
        }
        std::getline(_ifs, currLine, '\n');
        (currServer.*currDirective)(currLine);
    }
}