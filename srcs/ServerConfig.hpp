/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldavids <ldavids@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 11:30:54 by ldavids           #+#    #+#             */
/*   Updated: 2022/02/16 16:12:45 by ldavids          ###   ########.fr       */
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
#include "utils.hpp"

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
		
	public : 
		Location(std::string	block)
		{
			_method = set_vector("method", block);
			_redirection = set_string("redirection", block);
			_directory = set_string("directory", block);
			_default_file = set_string("default_file", block);
			_cgi = set_string("cgi", block);
			_upload_path = set_string("upload_path", block);
			_listing = set_int("listing", block);
		}
		~Location()
		{
		}
			std::vector<std::string>	get_method()
			{
				return (_method);
			}
			std::string					get_redirection()
			{
				return (_redirection);
			}
			std::string					get_directory()
			{
				return (_directory);
			}
			bool						get_listing()
			{
				return (_listing);
			}
			std::string					get_default_file()
			{
				return (_default_file);
			}
			std::string					get_cgi()
			{
				return (_cgi);
			}
			std::string					get_upload_path()
			{
				return (_upload_path);
			}			
};

class ServerConfig
{
	private:
		int										_port;					// one port per server
		int										_host[4];				// A server configuration can only have one host entry
															// If the server has to be available on multiple IP addresses, set the host to 0.0.0.0
		std::string								_host_name;				// Is it usefull?
		std::string								_server_names[50];
		int										_client_max_body_size;	// in Mb
		std::map<int, std::string>				_error_pages;			// error_nb linked to their location
		/*std::map<std::string, std::string>		_location;*/				// first string is the location, second is the block
		std::map<std::string, Location>			_location;
		// directives inside block are : method, redirection, directory, listing, default_file, CGI, upload_path

	public:
					ServerConfig();
					~ServerConfig();
					ServerConfig(const ServerConfig &src);
	/*Server			&operator=(const Server &src);*/

	int										get_port();
	int										*get_host();
	std::string								get_host_name();
	std::string								*get_server_names();
	int										get_client_max_body_size();
	std::map<int, std::string>				get_error_pages();
	std::map<std::string, Location>			get_location();
	void									set_port(int x);
	void									set_host(int x, int y);
	void									set_host_name(std::string	name);
	void									set_server_name(std::string	*names);
	void									set_client_max_body_size(int x);
	void									set_error_pages(std::map<int, std::string> temp);
	void									set_location(std::map<std::string, Location> temp);
};



#endif
