/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/14 08:46:39 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/14 10:09:23 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "ConfigServer.hpp"
#include "Location.hpp"

int		configFileParseWorkers(std::string configFile, Config *config) {
	size_t pos;
	size_t i;

	pos = configFile.find("worker_processes");
	while (!isdigit(configFile[pos]) && configFile[pos] != ';') { pos++; }
	i = pos;
	while (configFile[i] != ';') { i++; }
	config->setWorker(std::stoi(configFile.substr(pos, i)));
	
	if (configFile.find("events")) {
		pos = configFile.find("worker_connections");
		while (!isdigit(configFile[pos]) && configFile[pos] != ';') { pos++; }
		i = pos;
		while (configFile[i] != ';') { i++; }
		config->setWorkerConnections(std::stoi(configFile.substr(pos, i)));
	}

	return 0;
}

Config *configFileParser(int fd) {
	Config *config = new Config();
	char c[4096];
	std::string configFile;

	for (int i = 0; i < 4096; i++)
		c[i] = '\0';
	while (int ret = read(fd, c, 4096) > 0) {
		if (ret == -1) { return NULL; }
		configFile.append(c);
		for (int i = 0; i < 4096; i++)
			c[i] = '\0';
	}
	
	configFileParseWorkers(configFile, config);

	return config;
}