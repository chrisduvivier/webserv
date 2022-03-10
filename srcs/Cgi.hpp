#ifndef CGI_HPP
# define CGI_HPP

# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <iostream>
# include <string>

# include <iosfwd>
# include <sstream>

# include "stdio.h"
# include "utils.hpp"
# include "HttpRequest.hpp"
# include "ServerConfig.hpp"
# include "webserv.hpp"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif


class Cgi
{
	public:
		Cgi();
		Cgi(HttpRequest request, std::string path_to_cgi, ServerConfig serv);
		~Cgi();
		
		//template for fork and execute the cgi birary
		int     execute_cgi(HttpRequest request);
		void    init_var();
		void    set_var(HttpRequest request, std::string path_to_cgi, ServerConfig serv);
		void	set_env();
		void	free_env();
		void	print_env();
		std::string get_body() const;

	private:
		/* CGI Environment Variables */
		std::string	_CONTENT_LENGTH;	// The length of the request body in bytes made available by the input stream or -1 if the length is not known.
		std::string _GATEWAY_INTERFACE;	// The revision of the Common Gateway Interface that the server uses.
		std::string _PATH_INFO;			// Extra path information passed to a CGI program.
		std::string _PATH_TRANSLATED;	// The translated version of the path given by the variable PATH_INFO. (root of config + path_info)
		std::string _QUERY_STRING;		// The query information from requested URL (i.e., the data following "?").
		std::string _REMOTE_ADDR;		// The remote IP address of the client making the request;
										// this could be the address of an HTTP proxy between the server and the user.
		std::string _REMOTE_USER;		// The user's login, authenticated by the web server.
		std::string _REQUEST_METHOD;	// The HTTP request method used for this request.
		std::string _SCRIPT_FILENAME;		// The URL path (e.g., /cgi/program.cgi) of the script being executed.
		std::string _SCRIPT_NAME;		// The URL path (e.g., /cgi/program.cgi) of the script being executed.
		std::string _SERVER_NAME;		// The server's hostname or IP address.
		std::string _SERVER_PROTOCOL;	// The name and revision of the request protocol, e.g., "HTTP/1.1"
		std::string _SERVER_PORT;		// The port number of the host on which the server is listening.
		std::string _SERVER_SOFTWARE;	// The name and version of the server software that is answering the client request.
		// std::string _redirect_status;
		std::string	_body;
		bool		_error;
		char		*_args[3];
		char		*_env[15];
};

#endif
