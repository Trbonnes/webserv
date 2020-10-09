/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 10:32:53 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/09 14:36:45 by trbonnes         ###   ########.fr       */
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
};

#endif
