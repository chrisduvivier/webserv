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

		void	parse_startline(std::string	one_line);
		void	parse_header_line(std::string line);
		void	print() const;

		const std::string	get_method() const { return _method; };
		const std::string	get_url() const { return _url; };
		const std::string	get_version() const { return _version; };
		const std::map<std::string, std::string>	get_headers() const { return _headers; };
		
	private:
		std::string                                     _method;
		std::string                                     _url;
		std::string                                     _version;
		std::map<std::string, std::string>   			_headers;
};


// NOT WORKINg
// std::ostream& operator<<(std::ostream& os, const HttpRequest& obj)
// {
// 	os << "method: " << obj.get_method() << "\nurl: " << obj.get_url() << "\nversion: " << obj.get_version() << std::endl;
// 	return (os);
// }

#endif
