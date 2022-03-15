#include "ServerConfig.hpp"

//\\\\\\\\\\\\\\\\\\\\\\/LOCATION/\\\\\\\\\\\\\\\\\\\\\\\//
//////////////PUBLIC\\\\\\\\\\\\\\/

Location::Location()
{}

Location::Location(std::string	block)
{
	check_directives_location(block);
	_method = set_method("method", block);
	_redirection = set_string("redirection", block);
	_directory = set_string("directory", block);
	_default_file = set_string("default", block);
	_cgi = set_string("cgi", block);
	_upload_path = set_string("upload", block);
	_listing = set_int("listing", block);
}

Location::~Location()
{
}

Location::Location(const Location &src)
{
	*this = src;
}

Location &Location::operator=(const Location &src)
{
	this->_method = src._method;
	this->_redirection = src._redirection;
	this->_directory = src._directory;
	this->_listing = src._listing;
	this->_default_file = src._default_file;
	this->_cgi = src._cgi;
	this->_upload_path = src._upload_path;
	return (*this);
}

std::vector<std::string>	Location::get_method()
{
	return (_method);
}
std::string					Location::get_redirection()
{
	return (_redirection);
}
std::string					Location::get_directory()
{
	return (_directory);
}
bool						Location::get_listing()
{
	return (_listing);
}
std::string					Location::get_default_file()
{
	return (_default_file);
}
std::string					Location::get_cgi()
{
	return (_cgi);
}
std::string					Location::get_upload_path()
{
	return (_upload_path);
}

/////////////LOCATION\\\\\\\\\\\\\/
//////////////PRIVATE\\\\\\\\\\\\\\/
int		Location::check_first_word(std::string word, std::string block)
{
	int i = block.find(word);
	int y = i;
	i--;
	if (i != (int)std::string::npos)
	{
		while (i > 0 && block[i] != '\n')
		{
			/*std::cout << block[i] << std::endl;*/
			if (!isspace(block[i]))
				return ((int)std::string::npos);
			i--;
		}
	}
	else
		return ((int)std::string::npos);
	return (y);
}


void		Location::check_directives_location(std::string block)
{
	std::vector<std::string>	directives;
	std::string	word = "";
	size_t i = 0;
	int method = 0;
	int redirection = 0;
	int directory = 0;
	int listing = 0;
	int default_file = 0;
	int cgi = 0;
	int upload = 0;

	directives.push_back("");
	directives.push_back("method");
	directives.push_back("redirection");
	directives.push_back("directory");
	directives.push_back("listing");
	directives.push_back("default");
	directives.push_back("cgi");
	directives.push_back("upload");

while (i < block.size())
{
	word.clear();
	if (block[i] == '\n')
	{
		while (i < block.size() && (isspace(block[i]) || block[i] == '\n'))
			i++;
		while (i < block.size() && (!isspace(block[i]) && block[i] != '{' && block[i] != '}'))
		{
			word.push_back(block[i]);
			i++;
		}
		if (i == block.size())
			break ;
		if (std::find(directives.begin(), directives.end(), word) == directives.end())
		{
			std::cerr << "Config file : location scope : directive \"" << word << "\" not authorized\n";
			exit(1);
		}
		if (word.find("method") != std::string::npos)
			method++;
		if (word.find("redirection") != std::string::npos)
			redirection++;
		if (word.find("directory") != std::string::npos)
			directory++;
		if (word.find("listing") != std::string::npos)
			listing++;
		if (word.find("default") != std::string::npos)
			default_file++;
		if (word.find("cgi") != std::string::npos)
			cgi++;
		if (word.find("upload") != std::string::npos)
			upload++;
		if (method > 1 || redirection > 1 || directory > 1 || listing > 1 || default_file > 1 || cgi > 1 || upload > 1)
			error_exit("Config file : location scope : redudant directive\n");
		}
		i++;
	}
}

std::vector<std::string>		Location::set_method(std::string word, std::string block)
{
	std::vector<std::string>	vec;
	int i = check_first_word(word, block);
	int y = i;
	if (i == -1)
	{
		vec.push_back("GET");
		vec.push_back("POST");
		vec.push_back("DELETE");
		return (vec);
	}
	else
	{
		int x = 0;
		while (i < (int)block.size())
		{
			if (block[i] == '\n')
				break ;
			i++;
		}
		std::string	temp = block.substr(y, i);
		x = (int)temp.find("GET");
		if (x != -1)
		{
			vec.push_back("GET");
			temp.erase(x, 3);
		}
		x = (int)temp.find("POST");
		if (x != -1)
		{
			vec.push_back("POST");
			temp.erase(x, 4);
		}
		x = (int)temp.find("DELETE");
		if (x != -1)
		{
			vec.push_back("DELETE");
			temp.erase(x, 6);
		}
		i = (int)temp.find("method");
		i += 6;
		while (i < (int)temp.size() && temp[i] != '\n')
		{
			if (temp[i] == '\n')
				break ;
			if (!isspace(temp[i]))
				error_exit("Config file : location scope : invalid value inside method directive\n");
			i++;
		}
	}
	return (vec);
}

std::string		Location::set_string(std::string word, std::string block)
{
	int i = check_first_word(word, block);
	std::string	temp;

	if (i == -1)
		return (temp);
	else
	{
		while (isalnum(block[i]) && i < (int)block.size() && block[i] != '\n')
			i++;
		while (i < (int)block.size() && isspace(block[i]))
			i++;
		while (i < (int)block.size() && block[i] != '\n')
		{
			if (isspace(block[i]))
				break ;
			temp.push_back(block[i]);
			i++;
		}
		while (i < (int)block.size() && block[i] != '\n')
		{
			if (!isspace(block[i]))
				error_exit("Config file : location scope :multiple strings/values for one directive\n");
			i++;
		}
	}
	return (temp);
}

int			Location::set_int(std::string word, std::string block)
{
	int i = check_first_word(word, block);
	std::string temp;

	if (i == -1)
		return (0);
	else
	{
		i += 7;
		while (i < (int)block.size() && isspace(block[i]))
			i++;
		while (i < (int)block.size() && block[i] != '\n')
		{
			if (isspace(block[i]))
				break ;
			if (!isdigit(block[i]))
				error_exit("Config file : location scope : values for listing should be digit\n");
			temp.push_back(block[i]);
			i++;
		}
	}
	if (atoi(temp.c_str()) != 0 && atoi(temp.c_str()) != 1)
		error_exit("Config file : location scope : values for listing 1(yes) or 0(no)\n");
	while (i < (int)block.size() && block[i] != '\n')
	{
		if (!isspace(block[i]))
			error_exit("Config file : location scope : listing : multiple or incorrect values\n");
		i++;
	}
	return (atoi(temp.c_str()));
}

//\\\\\\\\\\\\\\\\\\\\/SERVER_CONFIG/\\\\\\\\\\\\\\\\\\\\\//

ServerConfig::ServerConfig()
{
}

ServerConfig::~ServerConfig()
{
}

ServerConfig::ServerConfig(const ServerConfig &src)
{
	*this = src;
}

ServerConfig			&ServerConfig::operator=(const ServerConfig &src)
{
	int i = 0;

	this->_port = src._port;
	while(i < 4)
	{
		this->_host[i] = src._host[i];
		i++;
	}
	i = 0;
	while (i < 50)
	{
		this->_server_names[i] = src._server_names[i];
		i++;
	}
	this->_client_max_body_size = src._client_max_body_size;
	this->_error_pages = src._error_pages;
	this->_location = src._location;
	return (*this);
}

int				ServerConfig::get_port()
{
	return (_port);
}

int				*ServerConfig::get_host()
{
	return (_host);
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

std::map<std::string, Location>		ServerConfig::get_location()
{
	return (_location);
}

void									ServerConfig::set_port(int x)
{
	_port = x;
}

void									ServerConfig::set_host(int x, int y)
{
	_host[y] = x;
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

void									ServerConfig::set_location(std::map<std::string, Location> temp)
{
	_location = temp;
}
