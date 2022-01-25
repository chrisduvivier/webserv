#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{
	std::cout << "--HttpRequest constructor called--" << std::endl;
}

HttpRequest::HttpRequest(char *buffer)
{
	std::string			line;
	std::stringstream	tmp_string_stream(buffer);
	bool 				first_line = true;

	while (std::getline(tmp_string_stream, line, '\n'))
	{
		if (first_line)
		{
			this->parse_startline(line);
			first_line = false;
		}
		else
		{
			this->parse_header_line(line);
		}
	}
	std::cout << "==========================================\n";
	this->print();
	std::cout << "==========================================\n";
}

void	HttpRequest::print() const
{
	std::cout << "method: " << this->get_method() << "\nurl: " << this->get_url() << "\nversion: " << this->get_version() << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
    	std::cout << it->first << " => " << it->second << '\n';
}

void	HttpRequest::parse_startline(std::string line)
{
	int  position = 0, lpost = 0;

	// Find request type
	position = line.find(' ');
	this->_method = line.substr(0, position);
	position++; //Skip character ' '

	lpost = position;
	// Find path
	position = line.find(' ', lpost);
	this->_url = line.substr(lpost, (position - lpost));
	position++; //Skip character ' '

	// Find HTTP version
	this->_version = line.substr(position);
}


void	HttpRequest::parse_header_line(std::string line)
{
	std::string	key = "";
    std::string	value = "";

    if (line.size() == 0) return;

    int pos_split = line.find(":", 0); //Look for separator ':'

    key = line.substr(0, pos_split);
    value = line.substr(pos_split + 1);

    this->_headers[key] = value;
}
