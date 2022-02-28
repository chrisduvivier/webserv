#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{
	// std::cout << "--HttpRequest constructor called--" << std::endl;
}

/*
**	parse 1 line at a time and store the content to the class.
*/
HttpRequest::HttpRequest(char *buffer)
{
	std::string			line;
	std::stringstream	tmp_string_stream(buffer);
	bool 				first_line = true;
	bool				reading_headers = true;

	while (reading_headers == true && std::getline(tmp_string_stream, line, '\n'))
	{
		line = line.substr(0, line.size()-1);		// remove the trailing `\r` char
		if (reading_headers)
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
			if (line.size() == 0)	// check the header-body delimeter (empty line)
				reading_headers = false;
		}
	}
	// read body
	while (std::getline(tmp_string_stream, line))
	{
		this->_body.append(line);
	}
	
	std::cout << "------------   PRINTING   ------------\n";
	this->print();
	std::cout << "-------------    DONE   --------------\n";
}

void	HttpRequest::print() const
{
	std::cout << "method: " << this->get_method() << "\nurl: " << this->get_url() << "\nversion: " << this->get_version() << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
    	std::cout << it->first << ": " << it->second << '\n';
	std::cout << "BODY: \n[" << this->_body << "]\n";
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

/* 
**	Read the Headers from HTTP Message. Empty input is rejected. leading space is removed from the value
*/
void	HttpRequest::parse_header_line(std::string line)
{
	std::string	key = "";
    std::string	value = "";

    if (line.size() == 0) return;

    int pos_split = line.find(":", 0); //Look for separator ':'
	if (pos_split == -1) return ;

    key = line.substr(0, pos_split);
    value = line.substr(pos_split + 1);

	size_t leading_space_pos = value.find(' ');
	if (leading_space_pos != std::string::npos)
		value = value.substr(leading_space_pos + 1);
    this->_headers[key] = value;
}
