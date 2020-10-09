/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 10:32:47 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/09 12:30:19 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(): _server(0), _worker(1), _workerConnections(1024) {
}

Config::Config(const Config &c) {
	_server.clear();
	_server = c._server;
	_worker = c._worker;
	_workerConnections = c._workerConnections;
}

Config::~Config() {
}

Config &Config::operator=(const Config &c) {
	_server.clear();
	_server = c._server;
	_worker = c._worker;
	_workerConnections = c._workerConnections;
	return *this;
}

