#include "config/ServerBlock.hpp"

#include "utils/Logger.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

ServerBlock::ServerBlock() {
}

ServerBlock::~ServerBlock() {
}

void    ServerBlock::addLocationBlock(LocationBlock newLocationBlock) {
    _locationBlocks[newLocationBlock.getPath()] = newLocationBlock;
}

std::vector<std::string> ServerBlock::_extractParams(std::string const & line) {
    std::vector<std::string>    params;

    for (size_t i = 0 ; i < line.size() ;) {
        std::string currWord = "";
        for (; std::isspace(line[i]) && i < line.size() ; i++);
        for (; !std::isspace(line[i]) && i < line.size() ; i++)
            currWord += line[i];
        if (currWord.size())
            params.push_back(currWord);
    }

    return (params);
}