#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

class HttpRequest
{
	public:
		HttpRequest(char *buffer, long len);

		parse_startline(std::string	one_line);
		
	private:
		std::string                                     _method;
		std::string                                     _url;
		std::string                                     _version;
		std::unordered_map<std::string, std::string>    _header;

};

#endif
