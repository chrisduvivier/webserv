#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <string>
# include <unistd.h>

# include "HttpRequest.hpp"
# include "webserv.hpp"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif


class Cgi
{
	public:
		Cgi();
		Cgi(HttpRequest request);
		
		//template for fork and execute the cgi birary
		int     execute_cgi(std::string path_to_cgi);
		void    init_var();
		void    set_var(HttpRequest request);

		std::string get_body() const;
		

	private:
		/* CGI Environment Variables */
		std::string _GATEWAY_INTERFACE;	// The revision of the Common Gateway Interface that the server uses.
		std::string _PATH_INFO;			// Extra path information passed to a CGI program.
		std::string _PATH_TRANSLATED;	// The translated version of the path given by the variable PATH_INFO.
		std::string _REMOTE_ADDR;		// The remote IP address of the client making the request;
										// this could be the address of an HTTP proxy between the server and the user.
		std::string _REMOTE_USER;		// The user's login, authenticated by the web server.
		std::string _REQUEST_METHOD;	// The HTTP request method used for this request.
		std::string _SCRIPT_NAME;		// The URL path (e.g., /cgi/program.cgi) of the script being executed.
		std::string _SERVER_NAME;		// The server's hostname or IP address.
		std::string _SERVER_PORT;		// The port number of the host on which the server is listening.
		std::string _SERVER_SOFTWARE;	// The name and version of the server software that is answering the client request.
		std::string _QUERY_STRING;		// The query information from requested URL (i.e., the data following "?").
		// std::string _redirect_status;
		
		std::string _body;
		char		*_env[14];
};

#endif
