/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseHeaders.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 18:55:18 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/01 16:46:10 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"


std::string ParseStdHeaders(std::string request, size_t pos) {
	std::string s;

	while (request[pos] && request[pos] != ':') { pos++; }
	pos++;
	while (request[pos] && request[pos] == ' ') { pos++; }
	while (request[pos] && request[pos] != '\n') {
		s.push_back(request[pos++]);
	}
	return s;
}

std::vector<std::string> ParseAcceptHeaders(std::string request, size_t pos) {
	std::vector<std::string> v;
	std::vector<std::string> tmpV;
	std::string s;
	std::string s2;
	std::string delimiter = ",";
	int q;
	size_t i = 0;

	while (request[pos] && request[pos] != ':') { pos++; }
	pos++;
	while (request[pos] && request[pos] == ' ') { pos++; }
	while (request[pos] && request[pos] != '\n') {
		s.push_back(request[pos++]);
	}

	while ((pos = s.find(delimiter)) != s.npos) {
		s2 = s.substr(0, pos);
		s.erase(0, pos);
		tmpV.push_back(s2);
		s2.clear();
	}
	s2 = s.substr(0, s.npos);
	s.erase(0, pos);
	tmpV.push_back(s2);
	s2.clear();
	s.clear();

	while (tmpV.size()) {
		i = 0;
		s = tmpV[i];
		pos = s.find(";");
		if (pos == s.npos)
			q = 1;
		else {
			std::string tmpS = s.substr(pos + 1, s.npos);
			q = std::stoi(tmpS);
		}
		for (size_t j = 1; j < tmpV.size(); j++) {
			int q2 = 0;
			s2 = tmpV[j];
			pos = s2.find(";");
			if (pos == s2.npos)
				q2 = 1;
			else {
				std::string tmpS = s2.substr(pos + 1, s2.npos);
				q2 = std::stoi(tmpS);
			}
			if (q2 > q) {
				q = q2;
				s = s2;
				i = j;
			}
		}
		s.erase(s.find(";"), s.npos);
		v.push_back(s);
		tmpV.erase(tmpV[i]);
		s.clear();
		s2.clear();
	}

	return v;
}

void ParseAcceptCharsets(Socket *socket, std::string request, size_t pos) {
	socket->setAcceptCharset(ParseAcceptHeaders(request, pos));
}

void ParseAcceptLanguage(Socket *socket, std::string request, size_t pos) {
	socket->setAcceptLanguage(ParseAcceptHeaders(request, pos));
}

void ParseAutorization(Socket *socket, std::string request, size_t pos) {
	socket->setAuthorization(ParseStdHeaders(request, pos));
}

void ParseContentLength(Socket *socket, std::string request, size_t pos) {
	socket->setContentLength(ParseStdHeaders(request, pos));
}

void ParseContentLocation(Socket *socket, std::string request, size_t pos) {
	socket->setContentLocation(ParseStdHeaders(request, pos));
}

void ParseContentType(Socket *socket, std::string request, size_t pos) {
	socket->setContentType(ParseStdHeaders(request, pos));
}

void ParseDate(Socket *socket, std::string request, size_t pos) {
	socket->setDate(ParseStdHeaders(request, pos));
}

void ParseHost(Socket *socket, std::string request, size_t pos) {
	socket->setHost(ParseStdHeaders(request, pos));
}

void ParseReferer(Socket *socket, std::string request, size_t pos) {
	socket->setReferer(ParseStdHeaders(request, pos));
}

void ParseTransferEncoding(Socket *socket, std::string request, size_t pos) {
	socket->setTransferEncoding(ParseStdHeaders(request, pos));
}

void ParseUserAgent(Socket *socket, std::string request, size_t pos) {
	socket->setUserAgent(ParseStdHeaders(request, pos));
}
