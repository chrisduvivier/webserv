#include "HttpResponse.hpp"


/* ---- UTILS FUNCTIONS ---- */

std::map<std::string, std::string> ContentTypeList(){
	std::map<std::string, std::string> contentType;
	contentType["html"] = "text/html; charset=utf-8";
	contentType["txt"] = "text/plain; charset=utf-8";
	contentType["jpeg"] = "image/jpeg";
	contentType["png"] = "image/png";
	contentType["gif"] = "image/gif";
	contentType["bmp"] = "image/bmp";
	contentType["webp"] = "image/webp";
	contentType["ico"] = "image/vnd.microsoft.icon";

	return (contentType);
}

/* --- Should be pass to the HttpResponse, this function is only for tests purpose --- */
#include "ConfigFile.hpp"
#include "ServerConfig.hpp"
ServerConfig getServerConf() {
	ConfigFile conf("./conf/default.conf");
	ServerConfig servConf = conf.populate(0);



	{							/* --- display conf --- */
		std::cout << "PORT : " <<  servConf.get_port() << std::endl;
		std::cout << "HOST : " << servConf.get_host() << std::endl;
		std::cout << "HOST NAME : " << servConf.get_host_name() << std::endl;
		for (size_t i = 0; i < 50; i++)
		{
			if (!servConf.get_server_names()[i].empty())
				std::cout << "SERVER NAME[" << i << "] : " << servConf.get_server_names()[i] << std::endl;
		}
		std::cout << "CLIENT MAX BODY SIZE : " << servConf.get_client_max_body_size() << std::endl;

		std::map<int, std::string> error_page = servConf.get_error_pages();
		for (std::map<int, std::string>::const_iterator it = error_page.begin(); it != error_page.end(); ++it)
			std::cout << "ERROR: code: " << it->first << " location: " << it->second << std::endl;

		std::map<std::string, std::string> location = servConf.get_location();
		for (std::map<std::string, std::string>::const_iterator it = location.begin(); it != location.end(); ++it)
			std::cout << "LOCATIONS : " << it->first << " | " << it->second << std::endl;
	}							/* -- display conf end ---*/




	return (servConf);
}

/* ---- CLASS FUNCTIONS ---- */

HttpResponse::HttpResponse() : 
	_protocol("HTTP/1.1")
{}

HttpResponse::HttpResponse(HttpRequest request) : 
	_protocol("HTTP/1.1")
{
	// rely on build_response for now. might merge that funtion to here in the future.
	(void) request;
}

std::string	HttpResponse::construct_response()
{
	std::stringstream tmp;
	std::string response;

	// first line
	tmp << this->_protocol << " " << this->_status_code << " " << this->_status_text << "\r\n";

	// add Headers
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
    	tmp << it->first << ": " << it->second << "\r\n";
	
	// add delimiter between headers and body. 
	tmp << "\r\n" << this->_body;

	response = tmp.str();
	return (response);
}

void	HttpResponse::build_response(HttpRequest request)
{
	if (request.get_method() == "GET")
	{
		this->handle_get_request(request);
	}
	else if (request.get_method() == "POST")
	{
		this->handle_post_request(request);
	}
	else if (request.get_method() == "DELETE")
	{
		this->handle_delete_request(request);
	}
	else
	{
		throw MyException("Exception: Unknown Method detected from request\n");
	}
}

void	HttpResponse::handle_get_request(HttpRequest request)
{
	/* probably going to be a server attribute */
	std::string directory = "./public_html";

	if (!request.get_url().empty())
	{
		/* we build the path to the resource requested */
		std::string path = directory + request.get_url();

		/* we check if the resource exists - Error 404 if it does not */
		std::ifstream resource(path.c_str());
		if (!resource)
		{
			this->_protocol = "HTTP/1.1";
			this->_status_code = 404;
			this->_status_text = "Not Found";
			this->_headers["Content-Type"] = "text/html; charset=utf-8";
			this->_body = "<!DOCTYPE html>\n<html>\n<title>404 Not Found</title>\n<body>\n<div>\n<H1>404 Not Found</H1>\n<p>Unable to find a representation of the requested resource</p>\n</div>\n</body>\n</html>";
			int body_size = this->_body.length();
			this->_headers["Content-Length"] = static_cast<std::ostringstream*>( &(std::ostringstream() << body_size) )->str();
			resource.close();


			getServerConf();

			return ;
		}

		this->_protocol = "HTTP/1.1";
		this->_status_code = 200;
		this->_status_text = "OK";
		this->_headers["Content-Type"] = ContentTypeList()[path.substr(path.find('.', 1) + 1)];

		std::stringstream buff;
		buff << resource.rdbuf();
		resource.close();
		this->_body = buff.str();
		int body_size = this->_body.length();
		this->_headers["Content-Length"] = static_cast<std::ostringstream*>( &(std::ostringstream() << body_size) )->str();
		// this->_headers["Content-Length"] = std::to_string(this->_body.length());

		std::cout << "-----------PRINT RESPONSE TO CLIENT-----------" << std::endl;
		this->print();
	}

}

void	HttpResponse::print(){

	std::cout << this->_protocol << " " << this->_status_code << " " << this->_status_text << std::endl;
	for(std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
	std::cout << std::endl;
	std::cout << this->_body << std::endl;
}

void	HttpResponse::handle_post_request(HttpRequest request)
{
	std::cout << "HANDLING POST REQUEST" << std::endl;
	
	/* probably going to be a server attribute */
	std::string directory = "./public_html";
	if (!request.get_url().empty())
	{
		/* we build the path to the resource requested */
		std::string path = directory + request.get_url();

		// check if request is a upload or a cgi request
		int is_cgi_request = true;	// TODO
		if (is_cgi_request == true)
		{
			Cgi cgi(request);
			int ret = cgi.execute_cgi(path);
			if (ret < 0)
			{
				// error from execve/CGI
				std::cerr << "cgi error \n";
			}

			// complete the response retrieve from the CGI
			this->_protocol = "HTTP/1.1";
			this->_status_code = 200;
			this->_status_text = "OK";
			int body_size = this->_body.length();
			this->_headers["Content-Type"] = ContentTypeList()[path.substr(path.find('.', 1) + 1)];
			this->_headers["Content-Length"] = static_cast<std::ostringstream*>( &(std::ostringstream() << body_size) )->str();
		}
		else
		{
			std::cout << "TODO: UPLOAD request\n";
		}
	}
}

void	HttpResponse::handle_delete_request(HttpRequest request){

	/* probably going to be a server attribute */
	std::string directory = "./public_html";

	if (!request.get_url().empty())
	{
		/* we build the path to the resource requested */
		std::string path = directory + request.get_url();

		/* we check if the resource exists - Error 404 if it does not */
		std::ifstream resource(path.c_str());
		if (!resource)
		{
			this->_protocol = "HTTP/1.1";
			this->_status_code = 404;
			this->_status_text = "Not Found";
			this->_headers["Content-Type"] = "text/html; charset=utf-8";
			this->_body = "<!DOCTYPE html>\n<html>\n<title>404 Not Found</title>\n<body>\n<div>\n<H1>404 Not Found</H1>\n<p>Unable to find a representation of the requested resource</p>\n</div>\n</body>\n</html>";
			int body_size = this->_body.length();
			this->_headers["Content-Length"] = static_cast<std::ostringstream*>( &(std::ostringstream() << body_size) )->str();
			resource.close();
			return ;
		}
		resource.close();
		this->_protocol = "HTTP/1.1";

		if (std::remove(path.c_str()) == 0)
		{
			this->_status_code = 204;
			this->_status_text = "No Content";
		}
		else
		{
			this->_status_code = 500;
			this->_status_text = "Internal Server Error";
		}
	}
}
