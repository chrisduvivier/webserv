/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldavids <ldavids@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 15:33:18 by ldavids           #+#    #+#             */
/*   Updated: 2022/02/16 16:41:26 by ldavids          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

ConfigFile::ConfigFile()
{
}

ConfigFile::ConfigFile(char *file)
{
	std::ifstream		inFile;
	std::stringstream	strStream;
	std::string			str;

	_server_nb = 0;
    inFile.open(file);
	if (!inFile)
	{
		std::cerr << "Error : can't open config file\n";
		exit(1);
	}
    strStream << inFile.rdbuf(); // read the file
	str = strStream.str(); // str holds the content of the file
	inFile.close();
    str = clean_config(str); // cleaning (whitespaces, comments etc) and storing the config file
	divide_servers(str); // dividing servers and checking the config file
	/*std::cout << _server[0] << std::endl;*/
	parse_config();
}

ConfigFile::~ConfigFile()
{
}

std::string	ConfigFile::clean_config(std::string str)
{
	std::string			line;
	std::string			config_file;
	std::stringstream	strStream(str);
	int					i;
	int					blank = 1;

	while (std::getline(strStream, line, '\n'))
	{
		i = 0;
		line = line.substr(0, line.find("#"));

		blank = 1;
		while (i != (int)line.size() && blank == 1)
		{
			if (!isspace(line[i]))
				blank = 0;
			i++;
		}
		if (blank == 1)
		{
			line.clear();
		}
		trim(line);
		if (line.size() != 0)
		{
			config_file.append(line.substr(i - 1, line.size() - i + 1));
			config_file.append("\n");
		}
	}
	return (config_file);
}

void ConfigFile::trim(std::string &s)
{
	const std::string WHITESPACE = " \r\t\f\v";

	size_t start = s.find_first_not_of(WHITESPACE);
	if (start != std::string::npos)
   		s.substr(start);
	size_t end = s.find_last_not_of(WHITESPACE);
	if (end != std::string::npos)
		s = s.substr(0, end + 1);
	start = s.find(";");
	if (start != std::string::npos)
	{
		s = s.substr(0, start);
	}
    return ;
}

void	ConfigFile::divide_servers(std::string str)
{
	size_t			i = str.find("server");
	size_t			temp;
	int				x = 0;

	while (i != std::string::npos)
	{
		temp = i;
		_server[x] = str.substr(i, str.size() - i);
		while (i != _server[x].size())
		{
			if (_server[x][i] == '{')
			{
				/*std::cout << "got a bracket" << std::endl;*/
				i++;
				break ;
			}
			i++;
		}
		if (i == _server[x].size())
		{
			_server[0] = str;
			return ;
		}
		int				brackets = 1;

		while (i != _server[x].size())
		{
			if (_server[x][i] == '{')
				brackets++;
			if (_server[x][i] == '}')
				brackets--;
			if (brackets == 0)
			{
				i++;
				break ;
			}
			i++;
		}
		if (i == _server[x].size())
		{
			std::cout << "brackets in config file not closed\n";
			exit(1);
		}
		str = _server[x].substr(i, _server[x].size() - i);
		_server[x] = _server[x].substr(0, i);
		i = str.find("server");
		/*std::cout << "\nSERVER " << x << std::endl;
		std::cout << _server[x] << std::endl;*/
		x++;
		_server_nb++;
	}
}

void	ConfigFile::parse_config()
{
	int		x = 0;

	while (x < _server_nb)
	{
		/*std::cout << "\nSERVER " << x << std::endl;
		std::cout << _server[x] << std::endl;*/
		listen(x);
		/*std::cout << "listen[x] == " << _listen[x] << std::endl;*/
		server_name(x);
		/*std::cout << "server_name[x][0] == " << _server_name[x][0] << std::endl;*/
		client_max_body_size(x);
		/*std::cout << "client_max_body_size[x] == " << _client_max_body_size[x] << std::endl;*/
		error_page(x);
		location(x);
		std::map<std::string, Location>::iterator it2;
		it2 = _location[x].begin();
		/*std::cout << "location = "<< it2->first << "\nblock = \n" << it2->second << std::endl;*/
		x++;
	}
}

void	ConfigFile::listen(int x)
{
	int	i = _server[x].find("listen");
	if (i == (int)std::string::npos)
	{
		std::cerr << "Error : no port to listen to in config file\n";
		exit(1);
	}
	i += 6;
	while (i < (int)_server[x].size())
	{
		if (!isspace(_server[x][i]))
			break ;
		i++;
	}
	while (_server[x][i] != '\n')
	{
		_listen[x].push_back(_server[x][i]);
		i++;
	}
}

void	ConfigFile::server_name(int x)
{
	int	y = 0;
	int	i = _server[x].find("server_name");

	if (i == (int)std::string::npos)
	{
		_server_name[x]->clear();
		return ;
	}
	i += 11;
	while (i < (int)_server[x].size())
	{
		if (!isspace(_server[x][i]))
			break ;
		i++;
	}
	while (_server[x][i] != '\n')
	{
		if (isspace(_server[x][i]) != 0)
		{
			i++;
			y++;
		}
		_server_name[x][y].push_back(_server[x][i]);
		i++;
	}
	/*i = 0;
	while (i <= y)
	{
		std::cout <<"server_name[" << x << "][" << y << "] == " << _server_name[x][i] << std::endl;
		i++;
	}*/
}

void	ConfigFile::client_max_body_size(int x)
{
	int	i = _server[x].find("client_max_body_size");
	if (i == (int)std::string::npos)
	{
		_client_max_body_size[x] = "1";
		return ;
	}
	i += 20;
	while (i < (int)_server[x].size())
	{
		if (!isspace(_server[x][i]))
			break ;
		i++;
	}
	while (_server[x][i] != '\n')
	{
		if (isspace(_server[x][i]) != 0)
			break ;
		_client_max_body_size[x].push_back(_server[x][i]);
		i++;
	}
}

void	ConfigFile::error_page(int x)
{
	std::string	nb;
	std::string path;
	int i = 0;

	while (i < (int)_server[x].size())
	{
		nb = "";
		path = "";
		i = _server[x].find("error_page", i);
		if (i == (int)std::string::npos)
			break ;
		i += 10;
		while (i < (int)_server[x].size())
		{
			if (!isspace(_server[x][i]))
				break ;
			i++;
		}
		while (_server[x][i] != '\n' && isalnum(_server[x][i]))
		{
			if (isspace(_server[x][i]) != 0)
				break ;
			nb.push_back(_server[x][i]);
			i++;
		}
		while (_server[x][i] != '\n')
		{
			while (isspace(_server[x][i]) != 0)
				i++;
			path.push_back(_server[x][i]);
			i++;
		}
		_error_pages[x].insert(std::pair<int, std::string>(atoi(nb.c_str()), path));
	}
	_error_pages[x].insert(std::pair<int, std::string>(400, "./public_html/400.html"));
	_error_pages[x].insert(std::pair<int, std::string>(404, "./public_html/404.html"));
	_error_pages[x].insert(std::pair<int, std::string>(405, "./public_html/405.html"));
	_error_pages[x].insert(std::pair<int, std::string>(500, "./public_html/500.html"));
	_error_pages[x].insert(std::pair<int, std::string>(501, "./public_html/501.html"));
	/*std::map<int, std::string>::iterator it1 = _error_pages[x].begin();
	std::cout << "errorpage nb == " << it1->first << " errorpage path == " << it1->second << std::endl;*/
}

void			ConfigFile::location(int x)
{
	std::string	key;
	std::string path;
	int i = 0;
	int brackets;

	while (i < (int)_server[x].size())
	{
		brackets = 1;
		key = "";
		path = "";
		i = _server[x].find("location", i);
		if (i == (int)std::string::npos)
			return ;
		i += 8;
		while (i < (int)_server[x].size())
		{
			if (!isspace(_server[x][i]))
				break ;
			i++;
		}
		while (_server[x][i] != '\n')
		{
			if (isspace(_server[x][i]) != 0)
				break ;
			key.push_back(_server[x][i]);
			i++;
		}
		while (_server[x][i] != '{')
			i++;
		i++;
		while (_server[x][i] && brackets != 0)
		{
			if (_server[x][i] == '}')
				brackets--;
			if (_server[x][i] == '{')
				brackets++;
			if (brackets == 0)
				break ;
			path.push_back(_server[x][i]);
			i++;
		}
		if (i == (int)_server[x].size())
		{
			std::cerr << "Error : brackets not closed in conf file\n";
			exit(1);
		}
		Location temp(path);
		_location[x].insert(std::pair<std::string, Location>(key, temp));
	}
}

ServerConfig	ConfigFile::populate(int x)
{
	ServerConfig	serv;
	std::string		temp;
	std::string		temp2;
	int i = 0;
	int y = 0;

	while (i < (int)_listen[x].size())
	{
		if (isdigit(_listen[x][i]))
			temp.push_back(_listen[x][i]);
		if (_listen[x][i] == '.')
		{
			serv.set_host(atoi(temp.c_str()), y);
			temp.clear();
			y++;
		}
		if (isalpha(_listen[x][i]))
			temp2.push_back(_listen[x][i]);
		i++;
	}
	serv.set_port(atoi(temp.c_str()));
	serv.set_host_name(temp2);
	serv.set_server_name(_server_name[x]);
	serv.set_client_max_body_size(atoi(_client_max_body_size[x].c_str()));
	serv.set_error_pages(_error_pages[x]);
	serv.set_location(_location[x]);
	return (serv);
}

int				ConfigFile::get_server_nb()
{
	return (_server_nb);
}
