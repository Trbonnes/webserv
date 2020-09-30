/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:45:46 by trbonnes          #+#    #+#             */
/*   Updated: 2020/09/30 17:32:11 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

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
	
	std::cout << s << std::endl;

	return socket;
}