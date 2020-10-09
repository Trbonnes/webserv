/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 12:24:49 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/09 12:24:50 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() : 
_location(""),
_root(""),
_allow(0),
_index(0),
_type(""),
_charset(""),
_language(0),
_auth_basic(""),
_auth_basic_user_file(""),
_autoindex("off") {}

Location::Location(std::string location, std::string root, std::vector<std::string> allow,
std::list<std::string> index, std::string type, std::string charset, std::vector<std::string> language,
std::string auth_basic, std::string auth_basic_user_file, std::string autoindex) :
_location(location),
_root(root),
_allow(allow),
_index(index),
_type(type),
_charset(charset),
_language(language),
_auth_basic(auth_basic),
_auth_basic_user_file(auth_basic_user_file),
_autoindex(autoindex) {}

Location::~Location() {}