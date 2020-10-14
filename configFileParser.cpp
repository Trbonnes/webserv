/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/14 08:46:39 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/14 12:09:31 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "ConfigServer.hpp"
#include "Location.hpp"

size_t	findClosingBracket(size_t pos, std::string configFile) {
	size_t	i;
	size_t	indentationLvl = 0;

	for (i = pos + 1; configFile[i]; i++) {
		if (configFile[i] == '}' && indentationLvl == 0)
			return i;
		if (configFile[i] == '}')
			indentationLvl--;
		if (configFile[i] == '{')
			indentationLvl++;
	}

	return configFile.npos;
}

int		configFileParseServerLocation(std::string parseServer, ConfigServer server) {
	size_t pos;
	size_t i;
	std::string s;

	if ((pos = parseServer.find("location")) == parseServer.npos)
		return 0;
	while (parseServer[pos] != ' ') { pos++; }
	while (parseServer[pos] == ' ') { pos++; }
	while (parseServer[pos] != ' ') {
		s.push_back(parseServer[pos++]);
	}


}

int		configFileParseServerUnit(std::string configFile, std::vector<ConfigServer> v) {
	size_t pos;
	size_t i;
	std::string parseServer;

	pos = configFile.find("server {");
	if ((i = findClosingBracket(configFile.find("{", pos), configFile)) == configFile.npos) {
		throw Config::InvalidConfigException();
	}

	parseServer = configFile.substr(pos, i - pos + 1);
	//std::cout << parseServer << std::endl;
	configFileParseServerLocation(parseServer, v.back());
	
	if ((pos = configFile.find("server {")) == configFile.npos)
		return 0;
	configFileParseServerUnit(configFile, v);

	return 0;
}

int		configFileParseServers(std::string configFile, Config *config) {
	std::vector<ConfigServer> v;
	size_t pos;
	size_t i;

	i = configFile.find("http");
	if (i == configFile.npos) {
		throw Config::InvalidConfigException();
	}
	configFile.erase(0, i);
	
	if ((pos = configFile.find("server {")) == configFile.npos) {
		throw Config::InvalidConfigException();
		return -1;
	}

	v.push_back(ConfigServer());
	configFileParseServerUnit(configFile, v);
	config->setServer(v);

	return 0;
}

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
		if (ret == -1) {
			throw Config::InvalidConfigException();
			return NULL;
	}
		configFile.append(c);
		for (int i = 0; i < 4096; i++)
			c[i] = '\0';
	}
	
	configFileParseWorkers(configFile, config);
	configFileParseServers(configFile, config);

	return config;
}