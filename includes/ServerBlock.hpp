#pragma once

#include <map>
#include <string>

class ServerBlock {
    private:
        std::string _host;
        int _port;
        //std::map<std::string, std::string>  _directives;

    public:
        ServerBlock();
        ~ServerBlock();

        void    setPortHost(std::string const word);
};