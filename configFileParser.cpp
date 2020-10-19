/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/14 08:46:39 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/19 08:43:42 by trbonnes         ###   ########.fr       */
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
	Location location;

	if ((pos = parseServer.find("location")) == parseServer.npos)
		return 0;
	if ((i = findClosingBracket(parseServer.find("{", pos), parseServer)) == parseServer.npos) {
		throw Config::InvalidConfigException();
	}
	s = parseServer.substr(pos, i - pos + 1);
	std::cout << s << std::endl;

	//LOCATION
	pos = 8;
	while (s[pos] == ' ') { pos++; }
	i = pos;
	while (s[i] != ' ') { i++; }
	location._location = s.substr(pos, i - pos);

	//METHOD
	if ((pos = s.find("method")) != s.npos) {
		i = s.find(";", pos);
		if (s.substr(pos, i - pos).find("GET") != s.npos)
			location._allow.push_back("GET");
		if (s.substr(pos, i - pos).find("HEAD") != s.npos)
			location._allow.push_back("HEAD");
		if (s.substr(pos, i - pos).find("POST") != s.npos)
			location._allow.push_back("POST");
		if (s.substr(pos, i - pos).find("PUT") != s.npos)
			location._allow.push_back("PUT");
		if (s.substr(pos, i - pos).find("DELETE") != s.npos)
			location._allow.push_back("DELETE");
		if (s.substr(pos, i - pos).find("CONNECT") != s.npos)
			location._allow.push_back("CONNECT");
		if (s.substr(pos, i - pos).find("OPTIONS") != s.npos)
			location._allow.push_back("OPTIONS");
		if (s.substr(pos, i - pos).find("TRACE") != s.npos)
			location._allow.push_back("TRACE");
	}

	//ROOT
	if ((pos = s.find("root")) != s.npos) {
		pos += 4;
		while (s[pos] == ' ') { pos++; }
		i = s.find(";", pos);
		location._root = s.substr(pos, i - pos);
	}

	//INDEX
	if ((pos = s.find("index")) != s.npos) {
		pos += 5;
		i = pos;
		while (s[i] != ';') {
			while (s[pos] == ' ') { pos++; }
			i = pos;
			while (s[i] != ' ' && s[i] != ';') { i++; }
			location._index.push_back(s.substr(pos, i - pos));
			pos = i;
		}
	}

	//AUTOINDEX
	if ((pos = s.find("auto_index")) != s.npos) {
		if ((i = s.find("on", pos)) != s.npos)
			location._autoindex = true;
		else if ((i = s.find("off", pos)) != s.npos)
			location._autoindex = false;
		else
			throw Config::InvalidConfigException();
	}

	//CLIENTBODYSIZE
	if ((pos = s.find("client_max_body_size")) != s.npos) {
		pos += 20;
		while (s[pos] == ' ') { pos++; }
		i = s.find(";", pos);
		location._clientBodySize = std::stoi(s.substr(pos, i - pos));
	}

	//ALIAS
	if ((pos = s.find("alias")) != s.npos) {
		pos += 5;
		while (s[pos] == ' ') { pos++; }
		i = s.find(";", pos);
		location._alias = s.substr(pos, i - pos);
	}

	//CGI
	if ((pos = s.find("cgi")) != s.npos) {
		pos += 3;
		std::string tmp;
		size_t j;
		i = s.find(";", pos);
		while (pos != i) {
			while (s[pos] == ' ') { pos++; }
			j = pos;
			while (s[j] &&s[j] != ' ' && s[j] != ';' && s[j] != '\n') { j++; }
			if (!s[j] || s[j] == '\n')
				throw Config::InvalidConfigException();
			location._cgi.push_back(s.substr(pos, j - pos));
			pos = j;
		}
	}

	std::cout << location._cgi.front() << std::endl;

	if ((pos = s.find("cgi_method")) != s.npos) {
		i = s.find(";", pos);
		if (s.substr(pos, i - pos).find("GET") != s.npos)
			location._cgi_allow.push_back("GET");
		if (s.substr(pos, i - pos).find("HEAD") != s.npos)
			location._cgi_allow.push_back("HEAD");
		if (s.substr(pos, i - pos).find("POST") != s.npos)
			location._cgi_allow.push_back("POST");
		if (s.substr(pos, i - pos).find("PUT") != s.npos)
			location._cgi_allow.push_back("PUT");
		if (s.substr(pos, i - pos).find("DELETE") != s.npos)
			location._cgi_allow.push_back("DELETE");
		if (s.substr(pos, i - pos).find("CONNECT") != s.npos)
			location._cgi_allow.push_back("CONNECT");
		if (s.substr(pos, i - pos).find("OPTIONS") != s.npos)
			location._cgi_allow.push_back("OPTIONS");
		if (s.substr(pos, i - pos).find("TRACE") != s.npos)
			location._cgi_allow.push_back("TRACE");
	}

	if ((pos = s.find("cgi_root")) != s.npos) {
		pos += 4;
		while (s[pos] == ' ') { pos++; }
		i = s.find(";", pos);
		location._cgi_root = s.substr(pos, i - pos);
	}


	server.insertLocation(location._location, location);
	return 0;
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
	std::cout << parseServer << std::endl;
	configFileParseServerLocation(parseServer, v.back());
	
	// if ((pos = configFile.find("server {")) == configFile.npos)
	// 	return 0;
	// return configFileParseServerUnit(configFile, v);

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