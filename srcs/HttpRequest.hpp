#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <map>
# include <vector>

# include "utils.hpp"

class HttpRequest
{
	public:

		HttpRequest();
		HttpRequest(char *buffer);
		// ~HttpRequest();
		
		HttpRequest (const HttpRequest &copy) 
		{ 
			this->_method = copy._method;
			this->_url = copy._url;
			this->_version = copy._version;
			this->_headers = copy._headers;
			this->_body = copy._body;
		}

		HttpRequest & operator = (const HttpRequest &rhs)
		{
			// TODO: deep copy this
			if (this != &rhs)
			{
				this->_method = rhs._method;
				this->_url = rhs._url;
				this->_version = rhs._version;
				this->_headers = rhs._headers;
				this->_body = rhs._body;
			}
			return *this;
		}

		void	parse_startline(std::string	one_line);
		void	parse_header_line(std::string line);
		void	parse_url(std::string url);
		
		void	print() const;

		const std::string							get_method() const { return _method; };
		const std::string							get_url() const { return _url; };
		const std::string							get_version() const { return _version; };
		const std::map<std::string, std::string>	get_headers() const { return _headers; };
		const std::string							get_body() const { return _body; };
		const std::string							get_query_string() const { return _query_string; };
		
	private:
		std::string                                     _method;
		std::string                                     _url;
		std::string                                     _version;
		std::map<std::string, std::string>   			_headers;
		std::string										_body;
		std::string										_query_string;
};

#endif
