/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPTools.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pforesti <pforesti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 14:34:27 by pforesti          #+#    #+#             */
/*   Updated: 2023/04/06 15:13:25 by pforesti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

enum HTTPMethod {
    GET,
    POST,
    DELETE
};

HTTPMethod strToM(std::string const str);
std::string mToStr(HTTPMethod const method);