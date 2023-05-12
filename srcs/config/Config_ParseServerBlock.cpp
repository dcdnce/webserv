#include "config/Config.hpp"

#include "utils/Logger.hpp"

#include <iostream>

// TODO 
//  - Parse Locations

void Config::_parseServerBlock() {
    ServerBlock    currServer;

    _parseServerBlockHeader(currServer);
    _parseServerBlockBody(currServer);
}

void Config::_parseServerBlockHeader(ServerBlock & currServer) {
    if (_getWord() != "server")
        throw std::runtime_error("Config::_parseServerBlockHeader: abort: \"server\" block not found");
    
    currServer.setPortHost(_getWord());

    if (_getWord() != "{")
        throw std::runtime_error("Config::_parseServerBlockHeader: abort: \"server\" block opening bracket error");

    #ifdef DEBUG
    	Logger::debug(true) << "Config::_parseServerBlockHeader: header read: " << "server " << currServer.getPortHost() << std::endl;
	#endif
}

void Config::_parseServerBlockBody(ServerBlock & currServer) {
    std::string currWord = " ";
    std::string currLine;

    while (true) {
        currWord = _getWord();

        if (currWord == "}")
            return ;

        if (_ifs.eof())
            throw std::runtime_error("Config::_parseServerBlockBody: abort: \"server\" block ending bracket error");
        
        //if == location

        ServerBlock::directiveFuncPtr currDirective = currServer.whichDirective(currWord);
        if (currDirective == NULL) {
            _skipToNewline();
            continue ;
        }
        std::getline(_ifs, currLine, '\n');
        (currServer.*currDirective)(currLine);

    }
}