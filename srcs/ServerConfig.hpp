/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldavids <ldavids@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 11:30:54 by ldavids           #+#    #+#             */
/*   Updated: 2022/02/14 16:10:46 by ldavids          ###   ########.fr       */
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
		std::map<std::string, std::string>		_location;				// first string is the location, second is the block
		// directives inside block are : method, redirection, root, listing, default_file, CGI, upload_path

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
	std::map<std::string, std::string>		get_location();
	void									set_port(int x);
	void									set_host(int x, int y);
	void									set_host_name(std::string	name);
	void									set_server_name(std::string	*names);
	void									set_client_max_body_size(int x);
	void									set_error_pages(std::map<int, std::string> temp);
	void									set_location(std::map<std::string, std::string> temp);
};



#endif
