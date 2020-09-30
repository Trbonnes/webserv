/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:45:46 by trbonnes          #+#    #+#             */
/*   Updated: 2020/09/30 18:11:54 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

int		httpRequestParseRequestLine(std::string s, Socket *socket) {
	char const *x[] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"};
	std::vector<std::string> methodsVec(x, x + sizeof(x) / sizeof(*x));
	std::string s2;
	size_t pos;

	//GET METHOD FROM REQUEST
	for (std::vector<std::string>::iterator i = methodsVec.begin(); i != methodsVec.end(); i++) {
		pos = s.find(*i);
		if (pos != std::string::npos)
			break ;
	}
	while (s[pos] && s[pos] != ' ') {
		s2.push_back(s[pos++]);
	}
	socket->setMethod(s2);
	s2.clear();

	//GET REQUEST-URI
	while (s[pos] && s[pos] != '/') { pos++ ;}
	while (s[pos] && s[pos] != ' ') {
		s2.push_back(s[pos++]);
	}
	socket->setRequestURI(s2);
	s2.clear();

	//GET HTTP VERSION
	while (s[pos] && s[pos] != 'H') { pos++; }
	while (s[pos] && s[pos] != '\n') {
		s2.push_back(s[pos++]);
	}
	socket->setHttpVersion(s2);
	s2.clear();

	return 0;
}

Socket	*httpRequestParser(int fd) {

	Socket *socket = new Socket(fd);
	char c[4096];
	std::string s;

	for (int i = 0; i < 4096; i++)
		c[i] = '\0';
	while (int ret = read(fd, c, 4096) > 0) {
		if (ret == -1) { return NULL; }
		s.append(c);
		for (int i = 0; i < 4096; i++)
			c[i] = '\0';
	}
	
	httpRequestParseRequestLine(s, socket);

	return socket;
}