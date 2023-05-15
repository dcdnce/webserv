#include "config/LocationBlock.hpp"

LocationBlock::LocationBlock() {

}

LocationBlock::~LocationBlock() {

}

void    LocationBlock::setPath(std::string const path) {
    this->_path = path;
}


std::string    LocationBlock::getPath(void) {
    return (_path);
}


std::vector<std::string> LocationBlock::_extractParams(std::string const & line) {
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

//301, 308, 302, 303, 307
bool    LocationBlock::_isHttpRedirectionCode(int const code) {
    return (code == 301 || code == 302 || code == 303 || code == 307 || code == 308);
}