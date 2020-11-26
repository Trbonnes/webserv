/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 10:32:53 by trbonnes          #+#    #+#             */
/*   Updated: 2020/11/11 16:57:46 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <list>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <vector>
# include <map>

# include "ConfigServer.hpp"

class	Config {
private:
	std::vector<ConfigServer>	_server;
	int							_worker;
	int							_workerConnections;
public:
	Config();
	Config(const Config &c);
	/*virtual*/ ~Config();
	Config &operator=(const Config &c);

	std::vector<ConfigServer>	&getServerList();
	ConfigServer				*getServerUnit(int port, std::string name);
	int							getWorker();
	int							getWorkerConnections();

	void						setServer(std::vector<ConfigServer> server);
	void						setWorker(int worker);
	void						setWorkerConnections(int worckerConections);

	class	InvalidConfigException: public std::exception {
		public:
			virtual const char* what() const throw();
	};
};

Config	*configFileParser(int fd);

#endif
