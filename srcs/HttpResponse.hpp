#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include <iostream>
# include <string>
# include <sstream>
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

		}

		HttpResponse & operator = (const HttpResponse &rhs)
		{
			// TODO: deep copy this
			if (this != &rhs)
			{
			}
			return *this;
		}
		
	private:
		std::string	_protocol;
		int			_status_code;
		std::string	_status_text;
		std::string	_response;

		
};

#endif
