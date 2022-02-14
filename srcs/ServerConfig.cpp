/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldavids <ldavids@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 12:18:06 by ldavids           #+#    #+#             */
/*   Updated: 2022/02/14 16:11:23 by ldavids          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : _port()
{
	_port = 8080;
	_host[0] = 127;
	_host[1] = 0;
	_host[2] = 0;
	_host[3] = 1;
	_host_name = "localhost";
	_server_names[0] = "9gag.com";
	_server_names[1] = "pronhubl.com";
	_client_max_body_size = 1; // 1 mb is default on nginx
	_error_pages.insert(std::make_pair(404, "/usr/share/nginx/html/custom_404.html"));
	_error_pages.insert(std::make_pair(500, "/usr/share/nginx/html/custom_500.html"));
	_location.insert(std::make_pair("/", "root /var/www/html \n method GET POST \n "));
	_location.insert(std::make_pair("/favicon.ico", "method GET"));
}

ServerConfig::~ServerConfig()
{
}

ServerConfig::ServerConfig(const ServerConfig &src)
{
	*this = src;
}

/*ServerConfig			&ServerConfig::operator=(const ServerConfig &src)
{
}*/

int				ServerConfig::get_port()
{
	return (_port);
}

int				*ServerConfig::get_host()
{
	return (_host);
}

std::string		ServerConfig::get_host_name()
{
	return (_host_name);
}

std::string		*ServerConfig::get_server_names()
{
	return (_server_names);
}

int				ServerConfig::get_client_max_body_size()
{
	return (_client_max_body_size);
}

std::map<int, std::string>		ServerConfig::get_error_pages()
{
	return (_error_pages);
}

std::map<std::string, std::string>		ServerConfig::get_location()
{
	return (_location);
}

void									ServerConfig::set_port(int x)
{
	/*std::cout << "set_port  = " << x <<std::endl;*/
	_port = x;
}

void									ServerConfig::set_host(int x, int y)
{
	_host[y] = x;
}

void									ServerConfig::set_host_name(std::string	host_name)
{
	_host_name = host_name;
}

void									ServerConfig::set_server_name(std::string	*names)
{
	int i = 0;

	while (i < 50)
	{
		_server_names[i] = names[i];
		i++;
	}
}

void									ServerConfig::set_client_max_body_size(int x)
{
	_client_max_body_size = x;
}

void									ServerConfig::set_error_pages(std::map<int, std::string> temp)
{
	_error_pages = temp;
}

void									ServerConfig::set_location(std::map<std::string, std::string> temp)
{
	_location = temp;
}
