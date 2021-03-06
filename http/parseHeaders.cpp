/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseHeaders.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yorn <yorn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 18:55:18 by trbonnes          #+#    #+#             */
/*   Updated: 2021/03/31 14:30:26 by yorn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

std::string ParseStdHeaders(std::string& request, size_t pos) {
	std::string s;

	while (request[pos] && request[pos] != ':') { pos++; }
	pos++;
	while (request[pos] && request[pos] == ' ') { pos++; }
	while (request[pos] && request[pos] != '\r') {
		s.push_back(request[pos++]);
	}
	return s;
}

std::vector<std::string> ParseAcceptHeaders(std::string& request, size_t pos) {
	std::vector<std::string> v;
	std::vector<std::string> tmpV;
	std::string s;
	std::string s2;
	std::string delimiter = ",";
	int q;

	while (request[pos] && request[pos] != ':') { pos++; }
	pos++;
	while (request[pos] && request[pos] == ' ') { pos++; }
	while (request[pos] && request[pos] != '\n') {
		s.push_back(request[pos++]);
	}

	while ((pos = s.find(delimiter)) != s.npos) {
		s2 = s.substr(0, pos);
		s.erase(0, pos + 1);
		tmpV.push_back(s2);
		s2.clear();
	}
	s2 = s.substr(0, s.npos);
	s.erase(0, pos);
	tmpV.push_back(s2);
	s2.clear();
	s.clear();
	while (tmpV.size()) {
		std::vector<std::string>::iterator tmpIt = tmpV.begin();
		std::vector<std::string>::iterator it = tmpIt;

		s = *tmpIt;
		pos = s.find(";");
		if (pos == s.npos)
			q = 1;
		else {
			pos = s.find("=");
			std::string tmpS = s.substr(pos + 1, s.npos);
			q = ft_atoi(tmpS.c_str());
		}
		for (tmpIt++; tmpIt != tmpV.end(); tmpIt++) {
			int q2 = 0;
			s2 = *tmpIt;
			pos = s2.find(";");
			if (pos == s2.npos)
				q2 = 1;
			else {
				pos = s2.find("=");
				std::string tmpS = s2.substr(pos + 1, s2.npos);
				q2 = ft_atoi(tmpS.c_str());
			}
			if (q2 > q) {
				q = q2;
				s = s2;
				it = tmpIt;
			}
		}
		if (s.find(";") != s.npos)
			s.erase(s.find(";"), s.npos);
		v.push_back(s);
		tmpV.erase(it);
		s.clear();
		s2.clear();
	}
	return v;
}

void ParseAcceptCharset(HttpRequest *socket, std::string& request, size_t pos) {
	socket->setAcceptCharset(ParseAcceptHeaders(request, pos));
}

void ParseAcceptLanguage(HttpRequest *socket, std::string& request, size_t pos) {
	socket->setAcceptLanguage(ParseAcceptHeaders(request, pos));
}

void ParseAutorization(HttpRequest *socket, std::string& request, size_t pos) {
	socket->setAuthorization(ParseStdHeaders(request, pos));
}

void ParseContentLength(HttpRequest *socket, std::string& request, size_t pos) {
	socket->setContentLength(ft_atoi(ParseStdHeaders(request, pos).c_str()));
}

void ParseContentLocation(HttpRequest *socket, std::string& request, size_t pos) {
	socket->setContentLocation(ParseStdHeaders(request, pos));
}

void ParseContentType(HttpRequest *socket, std::string& request, size_t pos) {
	//std::vector<std::string> v;
	std::string s = ParseStdHeaders(request, pos);

	//FOR MULPART BODY : NOT USEFUL IN OUR IMPLEMENTATION
	// pos = s.find("multipart");
	// if (pos == s.npos)
	// 	socket->setMultipartContent(false);
	// else {
	// 	socket->setMultipartContent(true);
	// 	pos = s.find("boundary=");
	// 	socket->setContentBoundary(s.substr(pos + 9, s.npos));
	// }
	// v.push_back(s);

	socket->setContentType(s);
}

void ParseDate(HttpRequest *socket, std::string& request, size_t pos) {
	socket->setDate(ParseStdHeaders(request, pos));
}

void ParseHost(HttpRequest *socket, std::string& request, size_t pos) {
	std::string host = ParseStdHeaders(request, pos);
	pos = host.find(":");
	pos++;
	socket->setPort(ft_atoi(host.substr(pos, host.npos).c_str()));
	host.erase(pos - 1, host.npos);
	socket->setHost(host);
}

void ParseReferer(HttpRequest *socket, std::string& request, size_t pos) {
	socket->setReferer(ParseStdHeaders(request, pos));
}

void ParseTransferEncoding(HttpRequest *socket, std::string& request, size_t pos) {
	socket->setTransferEncoding(ParseStdHeaders(request, pos));
}

void ParseUserAgent(HttpRequest *socket, std::string& request, size_t pos) {
	socket->setUserAgent(ParseStdHeaders(request, pos));
}

void ParseXSecret(HttpRequest *socket, std::string& request, size_t pos) {
	std::string s;
	pos++;
	while (request[pos] && request[pos] != '\n') {
		s.push_back(request[pos++]);
	}
	socket->setXSecret(s);
}