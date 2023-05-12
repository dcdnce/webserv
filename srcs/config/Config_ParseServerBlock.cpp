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

    #ifdef DEBUG
    	Logger::debug(true) << "Config::_parseServerBlockHeader: header read: " << "server " << std::endl;
	#endif
}

void Config::_parseServerBlockBody(ServerBlock & currServer) {
    std::string currWord = " ";
    std::string currLine;

    while (currWord != "}") {
        currWord = _getWord();

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
