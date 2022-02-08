/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldavids <ldavids@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 12:18:06 by ldavids           #+#    #+#             */
/*   Updated: 2022/02/08 13:58:00 by ldavids          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
{
	_port[0] = 8080;
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

int				*ServerConfig::get_port()
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
