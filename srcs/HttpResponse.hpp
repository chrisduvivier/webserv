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

# include "Cgi.hpp"

# include "ConfigFile.hpp"
# include "ServerConfig.hpp"

class HttpResponse
{
	public:

		HttpResponse();
		HttpResponse(HttpRequest request /*, ServerConfig serv */);
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
		void		build_response();
		void		handle_get_request();
		void		handle_post_request();
		void		handle_delete_request();

		void		print() const;
		int			check_method();
		int			check_redirection();
		std::string	get_location();
		std::string build_resource_path(int status);
		void		simple_response(int code, std::string status);
		void		simple_response(int code, std::string status, std::string path);
		std::string	get_response();


	private:
		std::string							_protocol;
		int									_status_code;
		std::string							_status_text;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		std::string							_response;
		ServerConfig						_serv;
		HttpRequest							_req;



};

#endif
