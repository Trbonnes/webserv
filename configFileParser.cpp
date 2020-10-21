/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configFileParser.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/14 08:46:39 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/21 09:36:11 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "ConfigServer.hpp"
#include "Location.hpp"

void	checkEndLine(size_t pos, std::string configFile) {
	size_t	i;

	for (i = configFile.find("\n", pos); pos < i; pos++) {
		if (configFile[pos] == ';')
			return ;
	}
	throw Config::InvalidConfigException();
}

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

std::string		configFileParseServerLocation(std::string parseServer, ConfigServer *server) {
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

	//LOCATION
	pos = 8;
	while (s[pos] == ' ') { pos++; }
	i = pos;
	while (s[i] != ' ') { i++; }
	location._location = s.substr(pos, i - pos);

	//METHOD
	if ((pos = s.find("	method")) != s.npos) {
		i = s.find(";", pos);
		checkEndLine(pos, s);
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
		checkEndLine(pos, s);
		while (s[pos] == ' ') { pos++; }
		i = s.find(";", pos);
		location._root = s.substr(pos, i - pos);
	}

	//INDEX
	if ((pos = s.find("	index")) != s.npos) {
		pos += 6;
		checkEndLine(pos, s);
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
		checkEndLine(pos, s);
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
		checkEndLine(pos, s);
		while (s[pos] == ' ') { pos++; }
		i = s.find(";", pos);
		location._clientBodySize = std::stoi(s.substr(pos, i - pos));
	}

	//ALIAS
	if ((pos = s.find("alias")) != s.npos) {
		pos += 5;
		checkEndLine(pos, s);
		while (s[pos] == ' ') { pos++; }
		i = s.find(";", pos);
		location._alias = s.substr(pos, i - pos);
	}

	//CGI
	if ((pos = s.find("cgi")) != s.npos) {
		pos += 3;
		checkEndLine(pos, s);
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

	if ((pos = s.find("cgi_method")) != s.npos) {
		i = s.find(";", pos);
		checkEndLine(pos, s);
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
		checkEndLine(pos, s);
		while (s[pos] == ' ') { pos++; }
		i = s.find(";", pos);
		location._cgi_root = s.substr(pos, i - pos);
	}

	server->insertLocation(location._location, location);

	pos = parseServer.find(s);
	i = pos;
	while (parseServer[i] != '{') { i++; }
	i = findClosingBracket(i, parseServer);
	parseServer.erase(pos, i - pos + 1);

	if (parseServer.find("location") != parseServer.npos) 
		return configFileParseServerLocation(parseServer, server);
	return parseServer;
}

int		configFileParseServerUnit(std::string configFile, std::vector<ConfigServer> *v) {
	size_t pos;
	size_t i;
	std::string parseServer;

	pos = configFile.find("server {");
	if ((i = findClosingBracket(configFile.find("{", pos), configFile)) == configFile.npos) {
		throw Config::InvalidConfigException();
	}

	v->push_back(ConfigServer());

	parseServer = configFile.substr(pos, i - pos + 1);
	parseServer = configFileParseServerLocation(parseServer, &(v->back()));

	//PORT
	if ((pos = parseServer.find("listen")) != parseServer.npos) {
		pos += 6;
		checkEndLine(pos, parseServer);
		while (parseServer[pos] == ' ') { pos++; }
		i = parseServer.find(";", pos);
		v->back().setPort(std::stoi(parseServer.substr(pos, i - pos)));
	}
	else
		throw Config::InvalidConfigException();

	//SERVER NAME
	if ((pos = parseServer.find("server_name")) != parseServer.npos) {
		std::string tmp;
		size_t j;
		std::vector<std::string> vs;
		pos += 11;
		checkEndLine(pos, parseServer);
		i = parseServer.find(";", pos);
		while (pos != i) {
			while (parseServer[pos] == ' ') { pos++; }
			j = pos;
			while (parseServer[j] &&parseServer[j] != ' ' && parseServer[j] != ';' && parseServer[j] != '\n') { j++; }
			if (!parseServer[j] || parseServer[j] == '\n')
				throw Config::InvalidConfigException();
			vs.push_back(parseServer.substr(pos, j - pos));
			pos = j;
		}
		v->back().setServer_name(vs);
	}
	else
		throw Config::InvalidConfigException();

	//METHOD
	if ((pos = parseServer.find("	method")) != parseServer.npos) {
		std::vector<std::string> allow;
		i = parseServer.find(";", pos);
		checkEndLine(pos, parseServer);
		if (parseServer.substr(pos, i - pos).find("GET") != parseServer.npos)
			allow.push_back("GET");
		if (parseServer.substr(pos, i - pos).find("HEAD") != parseServer.npos)
			allow.push_back("HEAD");
		if (parseServer.substr(pos, i - pos).find("POST") != parseServer.npos)
			allow.push_back("POST");
		if (parseServer.substr(pos, i - pos).find("PUT") != parseServer.npos)
			allow.push_back("PUT");
		if (parseServer.substr(pos, i - pos).find("DELETE") != parseServer.npos)
			allow.push_back("DELETE");
		if (parseServer.substr(pos, i - pos).find("CONNECT") != parseServer.npos)
			allow.push_back("CONNECT");
		if (parseServer.substr(pos, i - pos).find("OPTIONS") != parseServer.npos)
			allow.push_back("OPTIONS");
		if (parseServer.substr(pos, i - pos).find("TRACE") != parseServer.npos)
			allow.push_back("TRACE");
		v->back().setAllow(allow);
	}

	//ROOT
	if ((pos = parseServer.find("root")) != parseServer.npos) {
		pos += 4;
		checkEndLine(pos, parseServer);
		while (parseServer[pos] == ' ') { pos++; }
		i = parseServer.find(";", pos);
		v->back().setRoot(parseServer.substr(pos, i - pos));
	}

	//INDEX
	if ((pos = parseServer.find("	index")) != parseServer.npos) {
		std::vector<std::string> vi;
		pos += 6;
		checkEndLine(pos, parseServer);
		i = pos;
		while (parseServer[i] != ';') {
			while (parseServer[pos] == ' ') { pos++; }
			i = pos;
			while (parseServer[i] != ' ' && parseServer[i] != ';') { i++; }
			vi.push_back(parseServer.substr(pos, i - pos));
			pos = i;
		}
		v->back().setIndex(vi);
	}

	//AUTOINDEX
	if ((pos = parseServer.find("auto_index")) != parseServer.npos) {
		checkEndLine(pos, parseServer);
		if ((i = parseServer.find("on", pos)) != parseServer.npos)
			v->back().setAutoIndex(true);
		else if ((i = parseServer.find("off", pos)) != parseServer.npos)
			v->back().setAutoIndex(false);
		else
			throw Config::InvalidConfigException();
	}

	//CLIENTBODYSIZE
	if ((pos = parseServer.find("client_max_body_size")) != parseServer.npos) {
		pos += 20;
		checkEndLine(pos, parseServer);
		while (parseServer[pos] == ' ') { pos++; }
		i = parseServer.find(";", pos);
		v->back().setClientBodySize(std::stoi(parseServer.substr(pos, i - pos)));
	}

	//CGI
	if ((pos = parseServer.find("cgi")) != parseServer.npos) {
		pos += 3;
		checkEndLine(pos, parseServer);
		std::string tmp;
		size_t j;
		std::vector<std::string> vc;
		i = parseServer.find(";", pos);
		while (pos != i) {
			while (parseServer[pos] == ' ') { pos++; }
			j = pos;
			while (parseServer[j] &&parseServer[j] != ' ' && parseServer[j] != ';' && parseServer[j] != '\n') { j++; }
			if (!parseServer[j] || parseServer[j] == '\n')
				throw Config::InvalidConfigException();
			vc.push_back(parseServer.substr(pos, j - pos));
			pos = j;
		}
		v->back().setCGI(vc);
	}

	if ((pos = parseServer.find("cgi_method")) != parseServer.npos) {
		std::vector<std::string> vc;
		i = parseServer.find(";", pos);
		checkEndLine(pos, parseServer);
		if (parseServer.substr(pos, i - pos).find("GET") != parseServer.npos)
			vc.push_back("GET");
		if (parseServer.substr(pos, i - pos).find("HEAD") != parseServer.npos)
			vc.push_back("HEAD");
		if (parseServer.substr(pos, i - pos).find("POST") != parseServer.npos)
			vc.push_back("POST");
		if (parseServer.substr(pos, i - pos).find("PUT") != parseServer.npos)
			vc.push_back("PUT");
		if (parseServer.substr(pos, i - pos).find("DELETE") != parseServer.npos)
			vc.push_back("DELETE");
		if (parseServer.substr(pos, i - pos).find("CONNECT") != parseServer.npos)
			vc.push_back("CONNECT");
		if (parseServer.substr(pos, i - pos).find("OPTIONS") != parseServer.npos)
			vc.push_back("OPTIONS");
		if (parseServer.substr(pos, i - pos).find("TRACE") != parseServer.npos)
			vc.push_back("TRACE");
		v->back().setCGI_allow(vc);
	}

	if ((pos = parseServer.find("cgi_root")) != parseServer.npos) {
		pos += 4;
		checkEndLine(pos, parseServer);
		while (parseServer[pos] == ' ') { pos++; }
		i = parseServer.find(";", pos);
		v->back().setCGI_root(parseServer.substr(pos, i - pos));
	}

	//ERROR PAGES
	i = 0;
	if ((pos = parseServer.find("error_page", i)) != parseServer.npos) {
		while ((pos = parseServer.find("error_page", i)) != parseServer.npos) {
			std::string page;
			pos += 10;
			checkEndLine(pos, parseServer);
			while (parseServer[pos] == ' ') { pos++; }
			if ((i = parseServer.find("/", pos)) == parseServer.npos)
				throw Config::InvalidConfigException();
			size_t j = parseServer.find(";", i);
			page = parseServer.substr(i, j - i);
			while (pos != i) {
				j = pos;
				while (parseServer[j] != ' ' && j != i) { j++; }
				v->back().setErrorPages(std::stoi(parseServer.substr(pos, j - pos)), page);
				std::cout << v->back().getHTMLErrorPage(std::stoi(parseServer.substr(pos, j - pos))) << std::endl;
				pos = j;
				while (parseServer[pos] == ' ') { pos++; }
			}
		}
		std::cout << "OUT" << std::endl;
	}

	pos = configFile.find("server {");
	if ((i = findClosingBracket(configFile.find("{", pos), configFile)) == configFile.npos) {
		throw Config::InvalidConfigException();
	}
	configFile.erase(pos, i - pos + 1);

	if (configFile.find("server {") != configFile.npos) 
		return configFileParseServerUnit(configFile, v);
	return 0;
}

int		configFileParseServers(std::string configFile, Config *config) {
	std::vector<ConfigServer> v;
	size_t pos;
	size_t i;

	pos = configFile.find("http {");
	if (pos == configFile.npos) {
		throw Config::InvalidConfigException();
	}
	if ((i = findClosingBracket(configFile.find("{", pos), configFile)) == configFile.npos) {
		throw Config::InvalidConfigException();
	}
	configFile.erase(i);
	configFile.erase(0, pos + 7);
	
	if ((pos = configFile.find("server {")) == configFile.npos) {
		throw Config::InvalidConfigException();
	}

	configFileParseServerUnit(configFile, &v);
	config->setServer(v);

	return 0;
}

int		configFileParseWorkers(std::string configFile, Config *config) {
	size_t pos;
	size_t i;

	if ((pos = configFile.find("worker_processes")) == configFile.npos)
		throw Config::InvalidConfigException();
	checkEndLine(pos, configFile);
	while (!isdigit(configFile[pos]) && configFile[pos] != ';') { pos++; }
	i = pos;
	while (configFile[i] != ';') { i++; }
	config->setWorker(std::stoi(configFile.substr(pos, i)));
	
	if (configFile.find("events") != configFile.npos) {
		pos = configFile.find("worker_connections");
		checkEndLine(pos, configFile);
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
	
	std::cout << configFile << std::endl << std::endl;

	configFileParseWorkers(configFile, config);
	configFileParseServers(configFile, config);

	return config;
}