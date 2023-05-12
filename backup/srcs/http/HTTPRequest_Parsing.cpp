/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest_Parsing.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pforesti <pforesti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 14:23:19 by pforesti          #+#    #+#             */
/*   Updated: 2023/04/11 10:38:03 by pforesti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include <sstream>

static inline void parseFirstLine(HTTPRequest & hr, std::stringstream &ss);
static inline void parseHeaders(HTTPRequest & hr, std::stringstream &ss);
static inline void parseBody(HTTPRequest & hr, std::stringstream &ss);


void HTTPRequest::parse() {
    std::stringstream ss(_fullRequest);
        
    parseFirstLine(*this, ss);
    parseHeaders(*this, ss);
    parseBody(*this, ss);
}

static inline void parseFirstLine(HTTPRequest & hr, std::stringstream &ss) {
    std::string method, path, extension;

    ss >> method; 
    std::getline(ss, path, '.');
    ss >> extension;
    path += "." + extension;
    ss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    hr.setMethod(strToM(method));
    hr.setPath(path);
    hr.setExtension(extension);
}

static inline void parseHeaders(HTTPRequest & hr, std::stringstream &ss) {
    std::map<std::string, std::string> headers;
    std::string headerName, headerContent;

    while (std::getline(ss, headerName, ':') && std::getline(ss, headerContent, '\r') && ss.ignore(1, '\n'))
        headers[headerName] = headerContent;
        
    hr.setHeaders(headers);
}

static inline void parseBody(HTTPRequest & hr, std::stringstream &ss) {
    std::string body = "";
    
    for (std::string line; std::getline(ss, line);)
        body += line;
        
    hr.setBody(body);
}