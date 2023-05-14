#include "config/LocationBlock.hpp"

//TODO 
//  - defaults options in constructor

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