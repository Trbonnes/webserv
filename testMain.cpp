/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/14 09:20:21 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/14 10:52:20 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <fcntl.h>
#include "Config.hpp"

int main() {
    //size_t i = 0;
	int fd = open("./config/webserv.conf", O_RDWR);

	try {
		Config *config = configFileParser(fd);
		
		std::cout << config->getWorker() << std::endl;
		std::cout << config->getWorkerConnections() << std::endl;
	}
	catch (std::exception &e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}