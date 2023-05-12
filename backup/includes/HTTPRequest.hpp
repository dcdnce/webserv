/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pforesti <pforesti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 12:17:12 by pforesti          #+#    #+#             */
/*   Updated: 2023/04/11 10:30:38 by pforesti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <map>

#include "HTTPTools.hpp"

/*TODO
    - Specs error/exception
*/

class HTTPRequest {
    private:
        // - Members: method, path, headers, and body.
        std::string _fullRequest;
        HTTPMethod _method;
        std::string _path;
        std::string _extension;
        std::map<std::string, std::string> _headers;
        std::string _body;
        
    public:
        HTTPRequest();
        HTTPRequest(HTTPRequest const & src);
        HTTPRequest(std::string const & buffer);
        HTTPRequest(char* const buffer);
        ~HTTPRequest();
        
        // - Parsing
        // - Error/Validation: validation according to the HTTP protocol specifications.
        void    parse();

        //Getters/Setters
        void    setFullRequest(std::string const fullRequest);
        void    setMethod(HTTPMethod const method);
        void    setPath(std::string const path);
        void    setExtension(std::string const extension);
        void    setHeaders(std::map<std::string, std::string> const headers);
        void    setBody(std::string const body);

        std::string                             getFullRequest() const;
        HTTPMethod                              getMethod() const;
        std::string                             getPath() const;
        std::string                             getExtension() const;
        std::map<std::string, std::string>      getHeaders() const;
        std::string                             getBody() const;

        //Overloads
        HTTPRequest & operator=(HTTPRequest const & r);
};

std::ostream & operator<<(std::ostream & o, HTTPRequest const & r);