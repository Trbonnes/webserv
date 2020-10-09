/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trbonnes <trbonnes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 12:19:51 by trbonnes          #+#    #+#             */
/*   Updated: 2020/10/09 15:39:43 by trbonnes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGSERVER_HPP
# define CONFIGSERVER_HPP

# include <iostream>
# include <list>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <vector>
# include <map>

# include "Compare.hpp"
# include "Location.hpp"

class	ConfigServer {
private:
	std::map<std::string, Location, Compare<std::string> > _locationList;
    std::string                 _defaultRoot;
    std::vector<std::string>    _defaultAllow;
    std::string                 _defaultServerName;
    std::list<std::string>      _defaultIndex;
    std::string                 _defaultType;
    std::string                 _defaultCharset;
    std::vector<std::string>    _defaultLanguage;
    std::list<std::string>      _mimeTypes;
    std::string                 _configFilesRoot;
    std::string                 _defaultAuth_basic;
    std::string                 _defaultAuth_basic_user_file;
    std::string                 _defaultAutoindex;

    //PARSER VARIABLES
	std::string					_cgi;
	std::string					_cgi_method;
	std::string					_cgi_root;
    int                         _port;
    std::vector<std::string>    _server_name;
    std::string                 _root;
    std::vector<std::string>    _index;
    bool                        _autoIndex;
    int                         _clientBodySize;
    std::vector<std::string>    _acceptedMethods;

public:
	ConfigServer();
	ConfigServer(const ConfigServer &c);
	/*virtual*/ ~ConfigServer();
	ConfigServer &operator=(const ConfigServer &c);

    std::string                 getDefaultRoot(std::string _uri);
    std::string                 getDefaultServerName(std::string location);
    std::list<std::string>      &getDefaultIndex(std::string location);
    std::string                 getDefaultType(std::string location);
    std::string                 getDefaultCharset(std::string location);
    std::vector<std::string>    &getDefaultLanguage(std::string location);
    std::vector<std::string>    &getDefaultAllow(std::string location);
    std::list<std::string>      &getDefaultMimeTypes();
    std::string                 getDefaultLocation(std::string location);
    std::string                 getDefaultAuth_basic(std::string location);
    std::string                 getDefaultAuth_basic_user_file(std::string location);
    std::string                 getDefaultAutoindex(std::string location);

    //PARSER VARIABLES GETTERS
    std::string					getCgi();
	std::string					getCgi_method();
	std::string					getCgi_root();
    int                         getPort();
    std::vector<std::string>    getServer_name();
    std::string                 getRoot();
    std::vector<std::string>    getIndex();
    bool                        getAutoIndex();
    int                         getClientBodySize();
    std::vector<std::string>    getAcceptedMethods();

    //PARSER VARIABLES SETTERS
    void					setCgi(std::string _cgi);
	void					setCgi_method(std::string _cgi_method);
	void					setCgi_root(std::string _cgi_root);
    void                    setPort(int _port);
    void                    setServer_name(std::vector<std::string> _server_name);
    void                    setRoot(std::string _root);
    void                    setIndex(std::vector<std::string> _index);
    bool                    setAutoIndex(bool _autoIndex);
    void                    setClientBodySize(int _clientBodySize);
    void                    setAcceptedMethods(std::vector<std::string> _acceptedMethods);
};

#endif
