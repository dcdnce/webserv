#pragma once

#include "ServerBlock.hpp"
#include <fstream>
#include <vector>

class Config {
    private:
        std::ifstream _ifs;
        std::vector<ServerBlock>  _servers;

        void    _parsing();

        void    _parseServerBlock();
        void    _parseServerBlockHeader(ServerBlock & currServer);
        void    _parseServerBlockBody(ServerBlock & currServer);

        void    _parseDirectiveParameters();

        std::string    _getWord();

        void    _skipToSemicolon();

    public:
        Config(std::string const path);
        ~Config();

}; 