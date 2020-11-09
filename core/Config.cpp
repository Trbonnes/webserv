/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 10:32:47 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/19 15:49:03 by trbonnes         ###   ########.fr       */
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

std::vector<ConfigServer>	&Config::getServer() {
	return _server;
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
