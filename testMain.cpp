/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/14 09:20:21 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/21 10:05:59 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <fcntl.h>
#include "Config.hpp"

void	Location::printLocation() {
	std::cout << std::endl << "\033[0;33m///LOCATION///" << std::endl;

	std::cout << "\033[0;34m--LOCATION--" << std::endl;
	std::cout << _location << std::endl;
	std::cout << "--ALIAS--" << std::endl;
	std::cout << _alias << std::endl;
	std::cout << "--ROOT--" << std::endl;
	std::cout << _root << std::endl;
	std::cout << "--INDEX--" << std::endl;
	if (_index.size())
		for (std::vector<std::string>::iterator it = _index.begin(); it != _index.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--AUTO INDEX--" << std::endl;
	std::cout << _autoindex << std::endl;
	std::cout << "--CLIENT BODY--" << std::endl;
	std::cout << _clientBodySize << std::endl;
	std::cout << "--ALLOW--" << std::endl;
	if (_allow.size())
		for (std::vector<std::string>::iterator it = _allow.begin(); it != _allow.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--CGI--" << std::endl;
	if (_cgi.size())
		for (std::vector<std::string>::iterator it = _cgi.begin(); it != _cgi.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--CGI ALLOW--" << std::endl;
	if (_cgi_allow.size())
		for (std::vector<std::string>::iterator it = _cgi_allow.begin(); it != _cgi_allow.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--CGI ROOT--" << std::endl;
	std::cout << _cgi_root << std::endl;

}

void	ConfigServer::printServer() {
	std::cout << std::endl << "\033[0;31m///SERVER///" << std::endl;

	std::cout << "\033[0;32m--PORT--" << std::endl;
	std::cout << _port << std::endl;
	std::cout << "--SERVER NAME--" << std::endl;
	if (_defaultServerName.size())
		for (std::vector<std::string>::iterator it = _defaultServerName.begin(); it != _defaultServerName.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--ROOT--" << std::endl;
	std::cout << _defaultRoot << std::endl;
	std::cout << "--INDEX--" << std::endl;
	if (_defaultIndex.size())
		for (std::vector<std::string>::iterator it = _defaultIndex.begin(); it != _defaultIndex.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--AUTO INDEX--" << std::endl;
	std::cout << _defaultAutoindex << std::endl;
	std::cout << "--CLIENT BODY--" << std::endl;
	std::cout << _defaultClientBodySize << std::endl;
	std::cout << "--ALLOW--" << std::endl;
	if (_defaultAllow.size())
		for (std::vector<std::string>::iterator it = _defaultAllow.begin(); it != _defaultAllow.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--CGI--" << std::endl;
	if (_defaultCgi.size())
		for (std::vector<std::string>::iterator it = _defaultCgi.begin(); it != _defaultCgi.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--CGI ALLOW--" << std::endl;
	if (_defaultCgi_allow.size())
		for (std::vector<std::string>::iterator it = _defaultCgi_allow.begin(); it != _defaultCgi_allow.end(); it++) {
			std::cout << *it << std::endl;
		}
	std::cout << "--CGI ROOT--" << std::endl;
	std::cout << _defaultCgi_root << std::endl;

	std::cout << "--ERROR PAGES--" << std::endl;
	for (auto& x: _errorPages)
		std::cout << " [" << x.first << ':' << x.second << ']';
	std::cout << '\n';

	std::cout << "\033[0;35mLocation List: " << _locationList.size() << std::endl;
	
	std::cout << "Locations :";
	for (auto& x: _locationList)
		std::cout << " [" << x.first << ':' << x.second._location << ']';
	std::cout << '\n';

	for (std::map<std::string, Location, Compare<std::string> >::iterator it = _locationList.begin(); it != _locationList.end(); it++) {
			it->second.printLocation();
	}
}

int main() {
	int fd = open("./config/webserv.conf", O_RDWR);

	try {
		Config *config = configFileParser(fd);

		std::cout << std::endl;

		std::cout << "-> Workers: " << config->getWorker() << std::endl;
		std::cout << "-> Worker connections: " << config->getWorkerConnections() << std::endl;

		for (std::vector<ConfigServer>::iterator it = config->getServer().begin(); it != config->getServer().end(); it++) {
			it->printServer();
		}
		
	}
	catch (std::exception &e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}