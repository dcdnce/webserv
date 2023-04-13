/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pforesti <pforesti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 12:17:19 by pforesti          #+#    #+#             */
/*   Updated: 2023/04/11 10:32:33 by pforesti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

/* Constructors & destructors */
HTTPRequest::HTTPRequest() {}

HTTPRequest::HTTPRequest(HTTPRequest const & src) {*this = src;}
HTTPRequest::HTTPRequest(std::string const & buffer) : _fullRequest(buffer) {}
HTTPRequest::HTTPRequest(char* const buffer) : _fullRequest(buffer) {}
HTTPRequest::~HTTPRequest() {}

/* Getters & Setters */
void HTTPRequest::setFullRequest(std::string const fullRequest) {
    this->_fullRequest = fullRequest;
}
void HTTPRequest::setMethod(HTTPMethod const method) {
    this->_method = method;
}
void HTTPRequest::setPath(std::string const path) {
    this->_path = path;
}
void HTTPRequest::setExtension(std::string const extension) {
    this->_extension = extension;
}
void HTTPRequest::setHeaders(std::map<std::string, std::string> const headers) {
    this->_headers = headers;
}
void HTTPRequest::setBody(std::string const body) {
    this->_body = body;
}

std::string HTTPRequest::getFullRequest() const {
    return (this->_fullRequest);
}
HTTPMethod HTTPRequest::getMethod() const {
    return (this->_method);
}
std::string HTTPRequest::getPath() const {
    return (this->_path);
}
std::string HTTPRequest::getExtension() const {
    return (this->_extension);
}
std::map<std::string, std::string> HTTPRequest::getHeaders() const {
    return (this->_headers);
}
std::string HTTPRequest::getBody() const {
    return (this->_body);
}

/* Operators */
HTTPRequest & HTTPRequest::operator=(HTTPRequest const &r) {
    this->_fullRequest = r.getFullRequest();
    this->_method = r.getMethod();
    this->_path = r.getPath();
    this->_extension = r.getExtension();
    this->_headers = r.getHeaders();
    this->_body = r.getBody();
    return (*this);
}

std::ostream & operator<<(std::ostream & o, HTTPRequest const & r) {
    std::map<std::string, std::string> headers = r.getHeaders();
    
    o << mToStr(r.getMethod()) << " " << r.getPath() << std::endl;
    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        o << it->first << ":"<< it->second << std::endl; 
    o << r.getBody();

    return (o);
}