#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include <iostream>
# include <string>
# include <map>
# include <vector>

# include "utils.hpp"

class HttpRequest
{
	public:
		// Reading raw input from buffer
		HttpRequest(std::vector<std::string> client_request);

		void	parse_startline(std::string	one_line);
		
	private:
		std::string                                     _method;
		std::string                                     _url;
		std::string                                     _version;
		std::map<std::string, std::string>    _header;

};

#endif
