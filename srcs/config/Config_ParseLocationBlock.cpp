#include "config/Config.hpp"
#include "utils/Logger.hpp"

void Config::_parseLocationBlock(ServerBlock & currServer) {
    LocationBlock    currLocation;

    _parseLocationBlockHeader(currLocation);
    _parseLocationBlockBody(currLocation);
    //add new location to server object vector
}

void Config::_parseLocationBlockHeader(LocationBlock & currLocation) {
    currLocation.setPath(_getWord());
    
    if (_getWord() != "{")
        throw std::runtime_error("Config::_parseLocationBlockHeader: abort: block opening bracket error");

    #ifdef DEBUG
    	Logger::debug(true) << "Config::_parseLocationBlockHeader: header read: " << "location " << currLocation.getPath() << std::endl;
	#endif
}


void Config::_parseLocationBlockBody(LocationBlock & currLocation) {
    std::string currWord = " ";
    std::string currLine;

    while (true) {
        currWord = _getWord();

        // End of location block
        if (currWord == "}")
            return ;

        // No ending bracket
        if (_ifs.eof())
            throw std::runtime_error("Config::_parseLocationBlockBody: abort: \"server\" block ending bracket error");
        
        // Found directive
        LocationBlock::directiveFuncPtr currDirective = currLocation.whichDirective(currWord);
        if (currDirective == NULL) {
            _skipToNewline();
            continue ;
        }
        std::getline(_ifs, currLine, '\n');
        (currLocation.*currDirective)(currLine);
    }
}