/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldavids <ldavids@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 15:15:24 by cduvivie          #+#    #+#             */
/*   Updated: 2022/03/16 16:31:42 by ldavids          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() : 
	_method(""),
	_url(""),
	_version(""),
	_headers(),
	_body(""),
	_boundary(""),
	_query_string(""),
	_parsing_error_code(0)
{}

/*
**	Parse the Http Request from buffer, populate class attributes.
**	If the content type is a multipart (a.k.a chunk data), set the flags.
*/
void	HttpRequest::parse_request(char *buffer)
{
	std::string 	raw_input(buffer);
	size_t 			split_pos;
	if ((split_pos = raw_input.find("\r\n\r\n")) == std::string::npos)
	{
		this->_parsing_error_code = 400;
		return ;
		// throw ServerException("Exception HttpRequest: invalid request - Couldn't find header/body seperator");
	}

	// DEBUG("RAW HEADER = [" + raw_input.substr(0, split_pos) + "]")
	// DEBUG("RAW BODY = [" + raw_input.substr(split_pos + strlen("\r\n\r\n")) + "]")
	this->parse_headers(raw_input.substr(0, split_pos));
	this->parse_body(raw_input.substr(split_pos + strlen("\r\n\r\n"), raw_input.length()));
	this->parse_url(this->get_url());
	
	if (this->get_headers().count("Content-Type") && this->get_headers().at("Content-Type").find("multipart") != std::string::npos)
	{
		std::string content_type = this->get_headers().at("Content-Type");
		size_t pos = content_type.find("boundary=") + strlen("boundary=");
		std::string tmp = content_type.substr(pos);
		// DEBUG("BOUNDARY SRTING= [" + tmp + "]")
		_boundary = tmp;
	}
	
	// this->print();
	DEBUG("method: " + this->get_method() + "	url = " + this->get_url());
}

// populate map headers from reading raw header string
void	HttpRequest::parse_headers(std::string headers)
{
	bool 				first_line = true;
	std::string			line;
	std::stringstream	tmp_string_stream(headers);

	while (std::getline(tmp_string_stream, line, '\n'))
	{
		if (line.size() == 0)
			return ;
		line = line.substr(0, line.size()-1);		// remove the trailing `\r` char		
		if (first_line)
		{
			this->parse_startline(line);
			first_line = false;
		}
		else
			this->parse_header_line(line);
	}
}

// populate body from reading raw body string
void	HttpRequest::parse_body(std::string body)
{
	this->_body = body;
}

void	HttpRequest::print() const
{
	std::cout << YELLOW;
	std::cout << "---------  PRINTING REQUEST  ---------\n";
	std::cout << "method: " << this->get_method() << "\nurl: " << this->get_url() << "\nversion: " << this->get_version() << std::endl;
	std::cout << "query_string: " << this->get_query_string() << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
    	std::cout << it->first << ": " << it->second << '\n';
	std::cout << "BODY: \n[" << this->_body << "]\n";
	std::cout << "-------------    DONE   --------------\n\n";
	std::cout << RESET;
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

void	HttpRequest::parse_url(std::string url)
{
	std::string	new_url = "";
    std::string	query_string = "";

    if (url.size() == 0) return;

    int pos_split = url.find("?", 0); //Look for separator '?'
	if (pos_split == -1) return ;

    new_url = url.substr(0, pos_split);
    query_string = url.substr(pos_split + 1);
	
	this->_url = new_url;
	this->_query_string = query_string;
}
