#include "HttpResponse.hpp"

												/* ---- UTILS FUNCTIONS ---- */

/*
	--
		This function returns a map that contains ressources extension that are supported on the server as key
		and their corresponding Content-Type as value
	--
*/
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

/*
	--
		Checks if the path given as parameter leads to a directory
	--
*/
int	is_directory(const char *path) {

	std::cout << "is_directory IN" << std::endl;
	struct stat stats;

    stat(path, &stats);
    if (S_ISDIR(stats.st_mode))
        return 1;
    return 0;
}

void	print_location(Location locations) {

	std::vector<std::string> method = locations.get_method();
	std::cout << "		method      :";
	for (std::vector<std::string>::iterator i = method.begin(); i < method.end(); ++i)
	{
		std::cout << " " << *i;
	}
	std::cout << std::endl << "		redirection : " << locations.get_redirection() <<std::endl;
	std::cout << "		directory   : " << locations.get_directory() <<std::endl;
	std::cout << "		listing     : " << locations.get_listing() <<std::endl;
	std::cout << "		default file: " << locations.get_default_file() <<std::endl;
	std::cout << "		cgi         : " << locations.get_cgi() <<std::endl;
	std::cout << "		upload path : " << locations.get_upload_path() <<std::endl;
	

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
		std::cout << "LOCATIONS : " << std::endl;
		for (std::map<std::string, Location>::iterator it = location.begin(); it != location.end(); ++it)
		{
			// std::cout << "LOCATIONS : " << it->first << " | " << it->second.get_directory() << std::endl;
			std::cout << "	location : " << it->first << std::endl;
			print_location(it->second);
		}
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

	return (this->_response);
}

/*
	--
		This function assemble the different parts of the response to be
		understandable for the client
	--
*/
std::string	HttpResponse::construct_response() {

	std::stringstream tmp;
	std::string response;

	tmp << this->_protocol << " " << this->_status_code << " " << this->_status_text << "\r\n"; // first line

	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) // add Headers
    	tmp << it->first << ": " << it->second << "\r\n";
	
	tmp << "\r\n" << this->_body; // add delimiter between headers and body. 

	response = tmp.str();
	return (response);
}

/*
	--
		This function prints the response that will be sent to the client
	--
*/
void	HttpResponse::print() const {

	std::cout << "-- DEBUG -- HttpResponse --" << std::endl;
	std::cout << this->_protocol << " " << this->_status_code << " " << this->_status_text << std::endl;
	for(std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
	std::cout << std::endl;
	
	// std::cout << this->_body << std::endl;
}

/*
	--
		This function will operate the different processes that will lead to
		the creation of the appropriate response to the client
	--
*/
void	HttpResponse::build_response() {

	//Changes have to be made in check_redirection to make a proper Location (remove ./)
	//changes in Location ? or in code ? Or maybe write public_html instead of ./public_html ?


	int code = this->check_method();
	if (code == 200)
		code = this->check_redirection();
	std::map<int, std::string> error = _serv.get_error_pages();
	switch(code) {
	case 301:
		return (this->simple_response(301, "Moved Permanently"));
	case 400:
		return (this->simple_response(400, "Bad Request", error[400]));
	case 404:
		return (this->simple_response(404, "Not Found", error[404]));
	case 405:
		return (this->simple_response(405, "Not Allowed", error[405]));
	case 500:
		return (this->simple_response(500, "Internal Server Error", error[500]));
	case 501:
		return (this->simple_response(501, "Not Implemented", error[501]));
	}
	
	std::string path = this->build_resource_path(code);

	std::ifstream ifs(path);
	if (!ifs)
		return (this->simple_response(404, "Not Found", error[404]));
	if (!(is_directory(path.c_str())))
		return (this->simple_response(200, "OK", path));
	return (this->directory_response());
}

/*
	--
		1. Check if method is implemented on the server
		2. Check if method has mandatory headers
		3. Check if method is allowed on location
	--
*/
int	HttpResponse::check_method() {
	std::string method = _req.get_method();
	if (method != "POST" && method != "GET" && method != "DELETE")
		return (501);

	std::map<std::string, std::string> headers = _req.get_headers();
	if (method == "POST" && (headers["Content-Type"].empty() || headers["Content-Length"].empty()))
		return (400);
	std::string location = this->get_location();
	std::vector<std::string> allowed_method;
	allowed_method = _serv.get_location()[location].get_method();
	std::string allow;
	for (std::vector<std::string>::iterator it = allowed_method.begin(); it != allowed_method.end(); ++it)
	{
		if (it !=  allowed_method.begin())
			allow += ", ";
		allow += *it;
	}
	if (allow.find(method) == std::string::npos)
	{
		this->_headers["Allow"] = allow;
		return (405);
	}
	return (200);
}

/*
	--
		This function checks if the ressource requested by the client has been moved to 
		another directory and complete the HttpReponse header with the new url in that case
	--
*/
int	HttpResponse::check_redirection() {
	
	std::string location = this->get_location();
	if (this->_serv.get_location()[location].get_redirection().empty())
		return (200);
	
	this->_headers["Location"] = "http://localhost:8080" + this->build_resource_path(301); //Change hardcoded host
	// this->_headers["Location"] = this->_serv.get_host() + ':' + this->_serv.get_port() + this->build_resource_path(301); //smth like that
	return (301);
}

/*
	--
		This function will return the path to the location that contains the specific
		rules to correctly treat the requested ressource
	--
*/
std::string	HttpResponse::get_location() {

	std::string location = this->_req.get_url();
	std::map<std::string, Location> conf_location = this->_serv.get_location();

	while (!conf_location.count(location)) //check if map contains location as key
	{
		size_t pos = location.find_last_of("/");
		if (pos)
			location = location.substr(0, pos);
		else
			location = location.substr(0, pos + 1);
	}
	std::cout << "-- DEBUG --" << std::endl << "location: " << location << std::endl;
	return (location);
}

/*
	--
		This function will return the path to the ressource 
		that is requested by the client
	--
*/
std::string HttpResponse::build_resource_path(int status) {

	std::string path = this->_req.get_url();
	if (status == 301)
	{
		size_t to_erase = this->get_location().length();
		path.erase(0, to_erase + 1);
		path = this->_serv.get_location()[this->get_location()].get_redirection() + path;
	}
	else
		path = '.' + this->_serv.get_location()[this->get_location()].get_directory() + path;

	if (is_directory(path.c_str()) && path.back() != '/')
		path = path + '/';
	std::cout << "-- DEBUG --" << std::endl << "path: " << path << std::endl;

	return (path);
}

void	HttpResponse::simple_response(int code, std::string status) {

	this->_protocol = "HTTP/1.1";
	this->_status_code = code;
	this->_status_text = status;
	this->_response = this->construct_response();
	this->print();
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
	this->_response = this->construct_response();
	this->print();
}

void	HttpResponse::directory_response() {

	std::cout << "directory_response IN" << std::endl;
	std::string location = this->get_location();


	std::cout << "DEFAULT FILE: " << _serv.get_location()[location].get_default_file() << std::endl;
	if (!_serv.get_location()[location].get_default_file().empty())
	{
		std::cout << "directory_response Default" << std::endl;
		std::string path = _serv.get_location()[location].get_default_file();
		return (this->simple_response(200, "OK", path));
	}
	if (!_serv.get_location()[location].get_listing())
		return (this->simple_response(200, "OK"));

	/* --- building Index of HTML page -- */

	std::cout << "directory_response Listing 1" << std::endl;
	DIR *dp;
	struct dirent *ep;
	std::string body;

	dp = opendir (this->build_resource_path(200).c_str());
	if (dp != NULL)
	{
		body = body + "<!DOCTYPE html>\n<html>\n <body>\n  <h1>Index of " + this->_req.get_url() + "</h1>\n";
		body = body + "   <p>		______________________________________________________________________________________		</p>\n";
		body = body + "   </br></br></br>";
		while ((ep = readdir (dp)))
		{
			body = body + "  <a href=\"http://localhost:8080" + this->_req.get_url() + ep->d_name + "\">" + ep->d_name + "</a></br>";
			body += '\n';
		}
		body += " </body>\n</html>";
		if ((closedir(dp) == -1))
			return (this->simple_response(500, "Internal Server Error",  _serv.get_error_pages()[500]));
	}
	else
		return (this->simple_response(500, "Internal Server Error",  _serv.get_error_pages()[500]));

	std::cout << "=====		BODY	DEBUG		 	 =====" << std::endl;
	std::cout << body << std::endl;
	std::cout << "=====		BODY	DEBUG	END		 =====" << std::endl;

	this->_protocol = "HTTP/1.1";
	this->_status_code = 200;
	this->_status_text = "OK";

	this->_headers["Content-Type"] = "text/html; charset=utf-8";
	this->_body = body;
	int body_size = this->_body.length();
	this->_headers["Content-Length"] = static_cast<std::ostringstream*>( &(std::ostringstream() << body_size) )->str();
	this->_response = this->construct_response();
	this->print();
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
				this->_protocol = "HTTP/1.1";
				this->_status_code = 500;
				this->_status_text = "Internal Server Error";
				// TODO: redirect to 500.html error page?
				return ;
			}

			// complete the response retrieve from the CGI
			this->_protocol = "HTTP/1.1";
			this->_status_code = 200;
			this->_status_text = "OK";
			this->_body = cgi.get_body();
			int body_size = this->_body.length();
			this->_headers["Content-Type"] = "text/html";
			this->_headers["Content-Length"] = static_cast<std::ostringstream*>( &(std::ostringstream() << body_size) )->str();
		}
		else
		{
			std::cout << "TODO: UPLOAD request\n";
		}
	}
	std::cout << "-----------PRINT RESPONSE TO CLIENT-----------" << std::endl;
	this->print();
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