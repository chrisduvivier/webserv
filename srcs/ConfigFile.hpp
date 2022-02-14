/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldavids <ldavids@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 15:33:10 by ldavids           #+#    #+#             */
/*   Updated: 2022/02/14 16:18:53 by ldavids          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <map>
#include "ServerConfig.hpp"

class ConfigFile
{
	private:
		std::string								_server[50];
		int										_server_nb;
		std::string								_listen[50];
		std::string								_server_name[50][50];
		std::string								_client_max_body_size[50];
		std::map<int, std::string>				_error_pages[50];
		std::map<std::string, std::string>		_location[50];

		std::string		clean_config(std::string	str);
		void			divide_servers(std::string str);
		void			parse_config();
		void			trim(std::string &s);
		void			listen(int x);
		void			server_name(int x);
		void			client_max_body_size(int x);
		void			error_page(int x);
		void			location(int x);

	public:
					ConfigFile();
					ConfigFile(char *file);
					~ConfigFile();
	ServerConfig	populate(int x);
	int				get_server_nb();
};



#endif
