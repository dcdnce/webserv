#pragma once

#include "http/http.hpp"

#include <string>
#include <vector>
#include <set>

class LocationBlock {
    private:
        std::string _path;
        std::string _root;
        std::set<http::Method>   _acceptedMethods;

        std::vector<std::string> _extractParams(std::string const & line);

    public:
        LocationBlock();
        ~LocationBlock();

        typedef void (LocationBlock::*directiveFuncPtr)(std::string line);
        directiveFuncPtr    whichDirective(std::string const str);
        void    parseDirective_root(std::string line);
        void    parseDirective_acceptedMethods(std::string line);
        void    parseDirective_return(std::string line);
        void    parseDirective_autoindex(std::string line);
        void    parseDirective_index(std::string line);
        void    parseDirective_uploadedFilesPath(std::string line);
        void    parseDirective_cgi(std::string line);
        
        void    setPath(std::string const path);
        std::string    getPath(void);
};