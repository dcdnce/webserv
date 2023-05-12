#pragma once

#include <map>
#include <string>

class ServerBlock {
    private:
        std::string _host;
        int _port;
        std::string _serverName;

    public:
        ServerBlock();
        ~ServerBlock();

        typedef void (ServerBlock::*directiveFuncPtr)(std::string line);

        void    setPortHost(std::string const str);

        directiveFuncPtr    whichDirective(std::string const str);
        void    parseDirective_serverName(std::string line);
        void    parseDirective_errorPage(std::string line);
        void    parseDirective_clientMaxBodySize(std::string line);

        
};