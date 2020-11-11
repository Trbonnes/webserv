/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 10:32:47 by trbonnes          #+#    #+#             */
/*   Updated: 2020/11/11 16:58:24 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(): _server(0), _worker(1), _workerConnections(1024) {
}

Config::Config(const Config &c) {
	_server = c._server;
	_worker = c._worker;
	_workerConnections = c._workerConnections;
}

Config::~Config() {
}

Config &Config::operator=(const Config &c) {
	_server = c._server;
	_worker = c._worker;
	_workerConnections = c._workerConnections;
	return *this;
}

std::vector<ConfigServer>	&Config::getServerList() {
	return _server;
}

ConfigServer				*Config::getServerUnit(int port, std::string name) {
	for (size_t i = 0; i < _server.size(); i++) {
		std::vector<int> _port = _server[i].getPort();
		std::vector<std::string> _serverName = _server[i].getServerName();
		for (size_t j = 0; j < _port.size(); j++) {
			if (_port[j] == port) {
				for (size_t k = 0; k < _serverName.size(); k++) {
					if (_serverName[k] == name)
						return &(_server[i]);
				}
			}
		}
	}
	return NULL;
}

int							Config::getWorker() {
	return _worker;
}

int							Config::getWorkerConnections() {
	return _workerConnections;
}

void						Config::setServer(std::vector<ConfigServer> server) {
	_server = server;
}

void						Config::setWorker(int worker) {
	_worker = worker;
}

void						Config::setWorkerConnections(int worckerConections) {
	_workerConnections = worckerConections;
}

const char	*Config::InvalidConfigException::what() const throw() {
	return "Webserver configuration file is invalid";
}
