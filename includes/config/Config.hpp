#pragma once

#include "ServerBlock.hpp"
#include <fstream>
#include <vector>
#include <map>

class Config {
    private:
        std::ifstream _ifs;
        std::map<std::string, ServerBlock>  _serverBlocks;

        void    _parsing();
        void    _parseServerBlock();
        void    _parseServerBlockHeader(ServerBlock & currServer);
        void    _parseServerBlockBody(ServerBlock & currServer);
        void    _parseLocationBlock(ServerBlock & currServer);
        void    _parseLocationBlockHeader(LocationBlock & currLocation);
        void    _parseLocationBlockBody(LocationBlock & currLocation);

        void    _addServerBlock(ServerBlock const & newServerBlock);

        std::string    _getWord();

        void    _skipToNewline();


    public:
        Config(std::string const path);
        ~Config();


}; 