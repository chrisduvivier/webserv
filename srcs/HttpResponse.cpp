#include "HttpResponse.hpp"


/* ---- UTILS FUNCTIONS ---- */

std::map<std::string, std::string> ContentTypeList(){

	//Supported Type
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
		for (std::map<int, std::string>::iterator it = error_page.begin(); it != error_page.end(); ++it)
			std::cout << "ERROR: code: " << it->first << " location: " << it->second << std::endl;

		std::map<std::string, Location> location = servConf.get_location();
		for (std::map<std::string, Location>::iterator it = location.begin(); it != location.end(); ++it)
			std::cout << "LOCATIONS : " << it->first << " | " << it->second.get_directory() << std::endl;
	}							/* -- display conf end ---*/
	
	return (servConf);
}

/* ---- CLASS FUNCTIONS ---- */

HttpResponse::HttpResponse() : _protocol("HTTP/1.1") {}

HttpResponse::HttpResponse(HttpRequest request/*, ServerConfig serv*/) : _protocol("HTTP/1.1") {
	
	//remove comment when ServConfig will be passed to build HttpResponse in serverSocket
	this->_req = request;
	//this->_serv = serv;
	this->_serv = getServerConf();
}

std::string	HttpResponse::get_response() {

	this->_response = this->construct_response(); //to_change add construct in handling request without cgi
	return (this->_response);
}

std::string	HttpResponse::construct_response() {

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

void	HttpResponse::print() const {

	std::cout << this->_protocol << " " << this->_status_code << " " << this->_status_text << std::endl;
	for(std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
	std::cout << std::endl;
	std::cout << this->_body << std::endl;
}


void	HttpResponse::build_response()
{
	// if (this->_req.get_method() == "GET")
	// {
	// 	this->handle_get_request();
	// }
	// else if (this->_req.get_method() == "POST")
	// {
	// 	this->handle_post_request();
	// }
	// else if (this->_req.get_method() == "DELETE")
	// {
	// 	this->handle_delete_request();
	// }
	// else
	// {
	// 	this->check_method();
	// 	//throw MyException("Exception: Unknown Method detected from request\n");
	// }
	
	int code = this->check_method();

	std::map<int, std::string> error = _serv.get_error_pages();
	switch(code) {
	case 400:
		return (this->simple_response(400, "Bad Request", error[400]));
	case 405:
		return (this->simple_response(405, "Not Allowed", error[405]));
	case 500:
		return (this->simple_response(500, "Internal Server Error", error[500]));
	case 501:
		return (this->simple_response(501, "Not Implemented", error[501]));
	}

}

void	HttpResponse::handle_get_request()
{
	/* probably going to be a server attribute */
	std::string directory = "./public_html";

	if (!this->_req.get_url().empty())
	{
		/* we build the path to the resource requested */
		std::string path = directory + this->_req.get_url();

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

void	HttpResponse::handle_post_request()
{
	std::cout << "HANDLING POST REQUEST" << std::endl;
	
	/* probably going to be a server attribute */
	std::string directory = "./public_html";
	if (!this->_req.get_url().empty())
	{
		/* we build the path to the resource requested */
		std::string path = directory + this->_req.get_url();

		// check if request is a upload or a cgi request
		int is_cgi_request = true;	// TODO
		if (is_cgi_request == true)
		{
			Cgi cgi(this->_req);
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

void	HttpResponse::handle_delete_request(){

	/* probably going to be a server attribute */
	std::string directory = "./public_html";

	if (!this->_req.get_url().empty())
	{
		/* we build the path to the resource requested */
		std::string path = directory + this->_req.get_url();

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

int	HttpResponse::check_method() {

	std::string method = _req.get_method();
	if (method != "POST" && method != "GET" && method != "DELETE")
		return (501);
	
	std::map<std::string, std::string> headers = _req.get_headers();
	if (method == "POST" && (headers["Content-Type"].empty() || headers["Content-Length"].empty()))
		return (400);

	// std::string location = this->get_location();
	// std::vector<std::string> allowed_method;
	// allowed_method = _serv.get_location[location].get_method();
	// std::string allow;

	//Allow: GET, POST, DELETE
	// for (std::vector<std::string>::iterator it = allowed_method.begin(), it != allowed_method.end(), ++it)
	// {
	// 	if (it !=  allowed_method.begin())
	// 		allow += ", ";
	// 	allow += *it;
	// }

	return (500);
}

std::string	HttpResponse::get_location() {

	// std::string location = this->_req.get_url();
	// std::map<std::string, Location> conf_location = this->_serv.get_location();

	// while (!conf_location.count(location)) //check if map contains location as key
	// {
	// 	size_t pos = location.find_last_of("/");
	// 	location = location.substr(0, pos);
	// }
	// return (location);
}

void	HttpResponse::simple_response(int code, std::string status, std::string path) {

	std::ifstream resource(path.c_str());

	this->_protocol = "HTTP/1.1";
	this->_status_code = code;
	this->_status_text = status;
	this->_headers["Content-Type"] = ContentTypeList()[path.substr(path.find('.', 1) + 1)];

	std::stringstream buff;
	buff << resource.rdbuf();
	resource.close();
	this->_body = buff.str();
	int body_size = this->_body.length();
	this->_headers["Content-Length"] = static_cast<std::ostringstream*>( &(std::ostringstream() << body_size) )->str();
	
	this->print();
}