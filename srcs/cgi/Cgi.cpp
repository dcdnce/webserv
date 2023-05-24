#include "cgi/Cgi.hpp"

// ------------------------------------------------------------------ //
//  Constructors & Destructors                                        //
// ------------------------------------------------------------------ //
Cgi::Cgi() : _extension(""), _path("") {}
Cgi::Cgi(std::string const extension, std::string const path) : _extension(extension), _path(path) {}
Cgi::~Cgi(){}

// ------------------------------------------------------------------ //
//  Getters & Setters                                                 //
// ------------------------------------------------------------------ //
void	Cgi::setExtension(std::string const extension) {this->_extension = extension;}
void	Cgi::setPath(std::string const path) {this->_path = path;}
std::string	Cgi::getExtension() {return (_extension);}
std::string	Cgi::getPath() {return (_path);}