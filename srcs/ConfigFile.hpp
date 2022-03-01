/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldavids <ldavids@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 15:33:10 by ldavids           #+#    #+#             */
/*   Updated: 2022/03/01 16:55:22 by ldavids          ###   ########.fr       */
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
#include <algorithm>
#include <map>

#include "utils.hpp"
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
		std::map<std::string, Location>			_location[50];

		std::string		clean_config(std::string	str);	// cleaning (whitespaces, comments etc) and storing the config file
		void			divide_servers(std::string str);	// divide the string in the _server array
		void			trim(std::string &s);				// clean the whitespaces before and after each line + cleans the semicolon end everything behind it
		void			listen(int x);						// store string inside corresponding variables
		void			server_name(int x);					// store string inside corresponding variables
		void			client_max_body_size(int x);		// store string inside corresponding variables
		void			error_page(int x);					// store string inside corresponding variables
		void			location(int x);					// store string inside corresponding variables
		void			parse_config();						// loop for each server config
		int				keyword(std::string word, std::string str, int y); // checking if the word is a directive or a value (avoid getting false positives/bad find on directives)
																			// example : "server_name listening" could trigger an error cause the word listen is inside listening
		void			check_directives(int x); // used to check if the directives inside the server scope are correct (redundant or synthax)
												// location or cgi scope not included
		ServerConfig	listen_check(int x);	// check if synthax is correct for host and port
		int				client_max_body_size_check(std::string body_size);

	public:
					ConfigFile();
					ConfigFile(char *file);
					ConfigFile(ConfigFile &src);
	ConfigFile		&operator=(ConfigFile &src);
					~ConfigFile();
	ServerConfig	populate(int x);	// populate the ServerConfig object with the values extracted from the conf file
	int				get_server_nb();	// nb of servers
};

#endif
