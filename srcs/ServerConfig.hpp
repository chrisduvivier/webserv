/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldavids <ldavids@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 11:30:54 by ldavids           #+#    #+#             */
/*   Updated: 2022/03/15 14:56:43 by ldavids          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <map>
#include <algorithm>

#include "utils.hpp"

//\\\\\\\\\\\\\\\\\\\\\\/LOCATION/\\\\\\\\\\\\\\\\\\\\\\\//
//values inside the location scope (config file)
class Location
{
	private :
		std::vector<std::string>	_method;
		std::string					_redirection;
		std::string					_directory;
		int							_listing;
		std::string					_default_file;
		std::string					_cgi;
		std::string					_upload_path;

		void						check_directives_location(std::string block); // check if directives exist and not redundant
		std::string					set_string(std::string word, std::string block); // setter + some checks like one parameter only
		int							set_int(std::string word, std::string block); // setter + some checks like one parameter only + only 1 or 0
		std::vector<std::string>	set_method(std::string word, std::string block); // setter + some checks (GET - POST - DELETE only)
		int							check_first_word(std::string word, std::string block); // see if the searched word is a directive or just a parameter

	public :
		Location();
		Location(std::string	block);
		~Location();
		Location(const Location &src);
		Location &operator=(const Location &src);

		std::vector<std::string>	get_method();
		std::string					get_redirection();
		std::string					get_directory();
		bool						get_listing();
		std::string					get_default_file();
		std::string					get_cgi();
		std::string					get_upload_path();
};

//\\\\\\\\\\\\\\\\\\\\/SERVER_CONFIG/\\\\\\\\\\\\\\\\\\\\\//
//values inside the server scope (config file)
class ServerConfig
{
	private:
		int										_port;					// one port per server
		int										_host[4];				// A server configuration can only have one host entry
																		// If the server has to be available on multiple IP addresses, set the host to 0.0.0.0
		std::string								_server_names[50];
		int										_client_max_body_size;	// in Mb
		std::map<int, std::string>				_error_pages;			// error_nb linked to their location
		std::map<std::string, Location>			_location;
		// directives inside block are : method, redirection, directory, listing, default_file, CGI, upload_path

	public:
					ServerConfig();
					~ServerConfig();
					ServerConfig(const ServerConfig &src);
	ServerConfig	&operator=(const ServerConfig &src);

	int										get_port();
	int										*get_host();
	std::string								*get_server_names();
	int										get_client_max_body_size();
	std::map<int, std::string>				get_error_pages();
	std::map<std::string, Location>			get_location();
	void									set_port(int x);
	void									set_host(int x, int y);
	void									set_server_name(std::string	*names);
	void									set_client_max_body_size(int x);
	void									set_error_pages(std::map<int, std::string> temp);
	void									set_location(std::map<std::string, Location> temp);
};

#endif
