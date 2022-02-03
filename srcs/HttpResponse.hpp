#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <fstream>
# include <map>
# include <vector>

# include "utils.hpp"

# include "HttpRequest.hpp"
# include "MyException.hpp"

class HttpResponse
{
	public:

		HttpResponse();
		HttpResponse(HttpRequest request);
		// ~HttpResponse();
		
		HttpResponse (const HttpResponse &copy) 
		{
			this->_protocol = copy._protocol;
			this->_status_code = copy._status_code;
			this->_status_text = copy._status_text;
			this->_body = copy._body;
		}

		HttpResponse & operator = (const HttpResponse &rhs)
		{
			// TODO: deep copy this
			if (this != &rhs)
			{
				this->_protocol = rhs._protocol;
				this->_status_code = rhs._status_code;
				this->_status_text = rhs._status_text;
				this->_body = rhs._body;
			}
			return *this;
		}

		std::string	construct_response();
		void		build_response(HttpRequest request);
		void		handle_get_request(HttpRequest request);
		void		handle_post_request(HttpRequest request);

	private:
		std::string							_protocol;
		int									_status_code;
		std::string							_status_text;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

};

#endif
