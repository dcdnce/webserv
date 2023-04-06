/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPTools.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pforesti <pforesti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 14:33:51 by pforesti          #+#    #+#             */
/*   Updated: 2023/04/06 15:32:28 by pforesti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"HTTPTools.hpp"

HTTPMethod strToM(std::string const str) {
    if (str == "GET")
        return (HTTPMethod(GET));
    else if (str == "POST")
        return (HTTPMethod(POST));
    else if (str == "DELETE")
        return (HTTPMethod(DELETE));
    //else throw exception
    return (HTTPMethod(GET));
}

std::string mToStr(HTTPMethod const method) {
    if (method == GET)
        return ("GET");
    else if (method == POST)
        return ("POST");
    else if (method == DELETE)
        return ("DELETE");
    //else throw exception
    return ("");
}