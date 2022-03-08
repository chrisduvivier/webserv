/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 15:33:18 by ldavids           #+#    #+#             */
/*   Updated: 2022/03/08 12:05:10 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

////////////////CONFIGFILE\\\\\\\\\\\\\\\\\\/
/////////////////PUBLIC\\\\\\\\\\\\\\\\\\\\\/

ConfigFile::ConfigFile()
{
}

ConfigFile::ConfigFile(ConfigFile &src)
{
	*this = src;
}

ConfigFile		&ConfigFile::operator=(ConfigFile &src)
{
	return (src);
}

ConfigFile::ConfigFile(char *file)
{
	std::ifstream		inFile;
	std::stringstream	strStream;
	std::string			str;

	_server_nb = 0;
    inFile.open(file);
	if (!inFile)
		error_exit("Config file : can't open config file\n");
    strStream << inFile.rdbuf(); // read the file
	str = strStream.str(); // str holds the content of the file
	inFile.close();
    str = clean_config(str); // cleaning (whitespaces, comments etc) and storing the config file
	divide_servers(str); // dividing servers and checking the config file
	/*std::cout << _server[0] << std::endl;
	std::cout << _server[1] << std::endl;*/
	parse_config();
}

ConfigFile::~ConfigFile()
{
}

ServerConfig	ConfigFile::populate(int x)
{
	ServerConfig	serv;

	serv = listen_check(x); // contains "set_port and set_host" + check
	serv.set_server_name(_server_name[x]);
	serv.set_client_max_body_size(client_max_body_size_check(_client_max_body_size[x]));
	serv.set_error_pages(_error_pages[x]);
	serv.set_location(_location[x]);
	return (serv);
}

int				ConfigFile::get_server_nb()
{
	return (_server_nb);
}

/////////////////PRIVATE\\\\\\\\\\\\\\\\\\/

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
	size_t			i = str.size() - 1;
	size_t			temp;
	int				x = 0;

	while (i > 0 && str[i] != '}')
	{
		if (isspace(str[i]) == 0)
			error_exit("Please comment/delete text outside the server scope\n");
		i--;
	}
	i = str.find("server");
	while (i != std::string::npos)
	{
		temp = i;
		if (i > 0) // protection for oor
			i--;
		while (i > 0) // checking if text outside the server scope
		{
			if (isspace(str[i]) == 0)
				error_exit("Please comment text outside the server scope\n");
			i--;
		}
		i = temp;
		_server[x] = str.substr(i, str.size() - i);
		while (i != _server[x].size())
		{
			if (_server[x][i] == '{')
			{
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
			error_exit("brackets in config file not closed\n");
		str = _server[x].substr(i, _server[x].size() - i);
		_server[x] = _server[x].substr(0, i);
		i = str.find("server");
		x++;
		_server_nb++;
	}
}

void	ConfigFile::parse_config()
{
	int		x = 0;

	while (x < _server_nb)
	{
		/*std::cout << "\nSERVER " << x << std::endl;*/
		check_directives(x);
		/*std::cout << _server[x] << std::endl;*/
		listen(x);
		/*std::cout << "listen[x] == " << _listen[x] << std::endl;*/
		server_name(x);
		/*std::cout << "server_name[x][0] == " << _server_name[x][0] << std::endl;*/
		client_max_body_size(x);
		/*std::cout << "client_max_body_size[x] == " << _client_max_body_size[x] << std::endl;*/
		error_page(x);
		location(x);
		/*std::map<std::string, Location>::iterator it2;
		it2 = _location[x].begin();
		std::cout << "location = "<< it2->first << "\nblock = \n" << it2->second << std::endl;*/
		x++;
	}
}

void	ConfigFile::listen(int x)
{
	int	i = keyword("listen", _server[x], 0);
	if (i == (int)std::string::npos)
		error_exit("Error : no port to listen to in config file\n");
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
	int y = 0;
	int	i = keyword("server_name", _server[x], 0);
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
	int	i = keyword("client_max_body_size", _server[x], 0);
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
		i = keyword("error_page", _server[x], i);
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
			if (isalpha(_server[x][i]) != 0)
				error_exit("Config file : error in error_page directive\n");
			if (isspace(_server[x][i]) != 0)
				break ;
			nb.push_back(_server[x][i]);
			i++;
		}
		while (isspace(_server[x][i]) != 0)
				i++;
		while (_server[x][i] != '\n' && !isspace(_server[x][i]))
		{
			path.push_back(_server[x][i]);
			i++;
		}
		while (_server[x][i] != '\n' && i < (int)_server[x].size())
		{
			std::cout << _server[x][i];
			if (isalnum(_server[x][i]) != 0)
				error_exit("Config file : error in error_page directive\n");
			i++;
		}
		_error_pages[x].insert(std::pair<int, std::string>(atoi(nb.c_str()), path));
	}
	// if not all error pages are set up, insert the default one
	_error_pages[x].insert(std::pair<int, std::string>(400, "./public_html/error_pages/400.html"));
	_error_pages[x].insert(std::pair<int, std::string>(404, "./public_html/error_pages/404.html"));
	_error_pages[x].insert(std::pair<int, std::string>(405, "./public_html/error_pages/405.html"));
	_error_pages[x].insert(std::pair<int, std::string>(500, "./public_html/error_pages/500.html"));
	_error_pages[x].insert(std::pair<int, std::string>(501, "./public_html/error_pages/501.html"));
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
		i = keyword("location", _server[x], i);
		if (i == (int)std::string::npos) // create empty map to avoid segfault?
		{
			/*Location temp1("");
			_location[x].insert(std::pair<std::string, Location>("", temp1));*/
			return ;
		}
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
			error_exit("Config file : brackets not closed\n");
		Location temp(path);
		_location[x].insert(std::pair<std::string, Location>(key, temp));
	}
}

ServerConfig		ConfigFile::listen_check(int x)
{
	ServerConfig	serv;
	std::string		temp;
	int i = 0;
	int y = 0;

	while (i < (int)_listen[x].size() && isspace(_listen[x][i]))
		i++;
	if (isalpha(_listen[x][i])) // check if no unwanted chars before
		error_exit("Config file : error in listen directory\n");
	while (isdigit(_listen[x][i])) // get the port or host, depending if followed by '.' or not
	{
		temp.push_back(_listen[x][i]);
		i++;
		if (isalpha(_listen[x][i]))
			error_exit("Config file : error in listen directory\n");
	}
	if (_listen[x][i] == '.') // get the host (127.0.0.1 for ex)
	{
		while (y < 4)
		{
			while (isdigit(_listen[x][i]))
			{
				temp.push_back(_listen[x][i]);
				i++;
			}
			if (_listen[x][i] != '.' && _listen[x][i] != ':')
				error_exit("Config file : error in host\n");
			if (_listen[x][i] == ':' && y != 3)
				error_exit("Config file : error in host\n");
			serv.set_host(atoi(temp.c_str()), y);
			temp.clear();
			y++;
			i++;
		}
		i--;
		if (_listen[x][i] == ':') // get the port (127.0.0.1:80 for ex)
		{
			i++;
			while (isdigit(_listen[x][i]))
			{
				temp.push_back(_listen[x][i]);
				i++;
			}
		}
		else
			error_exit("Config file : no port specified\n");
	}
	else	// no host specified => default is 0.0.0.0
	{
		serv.set_host(0, 0);
		serv.set_host(0, 1);
		serv.set_host(0, 2);
		serv.set_host(0, 3);
	}
	while (i < (int)_listen[x].size()) // check if no unwanted chars after
	{
		if (!isspace(_listen[x][i]))
			error_exit("Config file : error in port\n");
		i++;
	}
	serv.set_port(atoi(temp.c_str()));
	return (serv);
}

int				ConfigFile::client_max_body_size_check(std::string body_size)
{
	size_t i = 0;

	while (i < body_size.size())
	{
		if (isalpha(body_size[i]))
			error_exit("Config file : error in client_max_body_size\n");
		i++;
	}
	return (atoi(body_size.c_str()));
}

int				ConfigFile::keyword(std::string word, std::string str, int y)
{
	int i = 0;
	int z = 0;
	while (i != -1)
	{

		i = y;
		i = i = str.find(word, y);
		if (i == -1)
			break ;
		y = i + 1;
		z = i;
		i--;
		while (i >= 0 && str[i] != '\n')
		{
			if (i >= 0 && str[i] == '\n')
			{
				y = -1;
				break ;
			}
			if (!isspace(str[i]))
				break ;
			i--;
		}
		if (y == -1 || (i > 0 && str[i] == '\n'))
			return (z);
	}
	return (-1);
}

void	ConfigFile::check_directives(int x)
{
	int			brackets = 0;
	size_t i = 0;
	std::string	word = "";
	int server_name = 0;
	int listen = 0;
	int client_max_body_size = 0;
	std::vector<std::string>	directives;

	directives.push_back("");
	directives.push_back("listen");
	directives.push_back("server_name");
	directives.push_back("client_max_body_size");
	directives.push_back("error_page");
	directives.push_back("location");
	directives.push_back("{");
	directives.push_back("}");

	while (i < _server[x].size() && _server[x][i] != '{')
			i++;
	i++;
	while (i < _server[x].size())
	{
		word.clear();
		if (_server[x][i] == '\n')
		{
			while (i < _server[x].size() && isspace(_server[x][i]))
				i++;
			while (i < _server[x].size() && (!isspace(_server[x][i]) && _server[x][i] != '{' && _server[x][i] != '}'))
			{
				word.push_back(_server[x][i]);
				i++;
			}
			if (i == _server[x].size())
				break ;
			if (std::find(directives.begin(), directives.end(), word) == directives.end())
			{
				std::cerr << "Config file : directive \"" << word << "\" not authorized in server scope\n";
				exit(1);
			}
			if (word.find("listen") != std::string::npos)
				listen++;
			if (word.find("server_name") != std::string::npos)
				server_name++;
			if (word.find("client_max_body_size") != std::string::npos)
				client_max_body_size++;
			if (listen > 1 || server_name > 1 || client_max_body_size > 1)
			{
				std::cerr << "Config file : redudant directive = ";
				if (listen > 1)
					std::cerr << "listen (use one line)\n";
				if (server_name > 1)
					std::cerr << "server_name (use one line)\n";
				else
					std::cerr << "multiple max body size\n";
				exit(1);
			}
			/*std::cout << "first word == " << word << std::endl;*/
		}
		if (_server[x][i] == '{')
		{
			brackets++;
			++i;
			while (brackets != 0)
			{
				if (_server[x][i] == '{')
					brackets++;
				if (_server[x][i] == '}')
					brackets--;
				if (brackets == 0)
					break ;
				i++;
			}
		}
		i++;
	}
}
