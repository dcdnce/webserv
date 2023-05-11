#pragma once

#include <fstream>

class Config {
    private:
        std::ifstream _ifs;
        void    _parsing();

    public:
        Config(std::string const path);
        ~Config();

}; 