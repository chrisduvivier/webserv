#include "HttpResponse.hpp"

HttpResponse::HttpResponse() : _protocol("HTTP/1.1") {}

HttpResponse::HttpResponse(HttpRequest request, ServerConfig serv) : _protocol("HTTP/1.1") {
	
	this->_req = request;
	this->_serv = serv;
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

void	HttpResponse::print() const {

	std::cout << "----- HttpResponse: _response -----" << std::endl;
	std::cout << this->_protocol << " " << this->_status_code << " " << this->_status_text << std::endl;
	for(std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
	std::cout << std::endl;
	/*std::cout << this->_body << std::endl;*/
}

/*
	--
		This function will operate the different processes that will lead to
		the creation of the appropriate response to the client
	--
*/
void	HttpResponse::build_response() {

	std::map<int, std::string> error = _serv.get_error_pages();

	int code = this->check_method();
	switch(code) {
	case 301:
		return (this->simple_response(301, "Moved Permanently"));
	case 400:
		return (this->simple_response(400, "Bad Request", error[400]));
	case 404:
		return (this->simple_response(404, "Not Found", error[404]));
	case 405:
		return (this->simple_response(405, "Not Allowed", error[405]));
	case 413:
		return (this->simple_response(413, "Payload Too Large", error[413]));
	case 500:
		return (this->simple_response(500, "Internal Server Error", error[500]));
	case 501:
		return (this->simple_response(501, "Not Implemented", error[501]));
	}

	std::string path = this->build_ressource_path();
	std::string extension = get_file_ext(path);

	if (extension == "cgi") //handling CGI
	{
		Cgi cgi(this->_req, path, this->_serv);
		int ret = cgi.execute_cgi();
		if (ret < 0)
			return (this->simple_response(500, "Internal Server Error", error[500]));
		this->_body = cgi.get_body();	//read cgi execution's output
		// header is added here manually for now
		this->_headers["Content-Type"] = "text/html; charset=utf-8";
		this->_headers["Content-Length"] = SSTR(this->_body.length());
		// std::cout << "EXECVE : FINISH! => body=[" << this->_body << "]\n\n";
		return (this->simple_response(200, "OK"));
	}

	if ((is_directory(path.c_str())))
		return (this->directory_response());

	if (_req.get_method() == "POST")
		return (this->handle_post());
	else if (_req.get_method() == "DELETE")
		return (this->handle_delete());
	
	return (this->simple_response(200, "OK", path));
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
	return (this->check_redirection());
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
		return (this->check_max_body_size());
	
	this->_headers["Location"] = "http://localhost:" + itostr(_serv.get_port()) + this->build_ressource_path();
	return (301);
}

/*
	--
		This function will verify the request body does not exceed the size specified in the conf file
	--
*/
int	HttpResponse::check_max_body_size() {

	size_t client_max_size = (size_t)this->_serv.get_client_max_body_size();
	if (client_max_size != 0 && client_max_size < this->_req.get_body().length())
		return (413);
	return (this->check_url_exists());
}

/*
	--
		This function will verify that the ressource requested exists
	--
*/
int	HttpResponse::check_url_exists() {

	std::string path = this->build_ressource_path();

	std::ifstream ifs(path.c_str());
	if (!ifs && _req.get_method() != "POST")
		return (404);
	return (200);
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

	while (!conf_location.count(location))
	{
		size_t pos = location.find_last_of("/");
		if (pos)
			location = location.substr(0, pos);
		else
			location = location.substr(0, pos + 1);
	}
	return (location);
}

/*
	--
		This function will return the path to the ressource 
		that is requested by the client
	--
*/
std::string HttpResponse::build_ressource_path() {

	std::string path = this->_req.get_url();
	std::string location = this->get_location();
	if (!this->_serv.get_location()[location].get_redirection().empty())
	{
		size_t to_erase = location.length();
		path.erase(0, to_erase + 1);
		path = this->_serv.get_location()[location].get_redirection() + path;
	}
	else
	{
		if (location != "/")
		{
			size_t to_erase = path.find(location);
			to_erase += location.length();
			path.erase(0, to_erase + 1);
		}
		else
			path.erase(0, 1);
		path = this->_serv.get_location()[location].get_directory() + path;
	}
	std::cout << "----- DEBUG_PATH: " << path << std::endl;
	return (path); 
}

void	HttpResponse::simple_response(int code, std::string status) {

	this->_protocol = "HTTP/1.1";
	this->_status_code = code;
	this->_status_text = status;
	this->_response = this->construct_response();
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
	this->_headers["Content-Length"] = SSTR(body_size);
	this->_response = this->construct_response();
}

void	HttpResponse::directory_response() {

	// std::cout << "directory_response IN" << std::endl;
	std::string location = this->get_location();

	if (_serv.get_location()[location].get_listing())
	{
		/* --- building 'Index of' HTML page -- */

		std::cout << "directory_response Listing 1" << std::endl;
		DIR *dp;
		struct dirent *ep;
		std::string body;

		dp = opendir (this->build_ressource_path().c_str());
		if (dp != NULL)
		{
			std::string url = this->_req.get_url();
			if (url[url.length() - 1] != '/')
				url += '/';
			body = body + "<!DOCTYPE html>\n<html>\n <body>\n  <h1>Index of " + url + "</h1>\n";
			body = body + "   <p>		______________________________________________________________________________________		</p>\n";
			body = body + "   </br></br></br>";
			while ((ep = readdir (dp)))
			{
				body = body + "  <a href=\"http://localhost:" + itostr(_serv.get_port()) + url + ep->d_name + "\">" + ep->d_name + "</a></br>";
				body += '\n';
			}
			body += " </body>\n</html>";
			if ((closedir(dp) == -1))
				return (this->simple_response(500, "Internal Server Error",  _serv.get_error_pages()[500]));
		}
		else
			return (this->simple_response(500, "Internal Server Error",  _serv.get_error_pages()[500]));

		this->_protocol = "HTTP/1.1";
		this->_status_code = 200;
		this->_status_text = "OK";
		this->_headers["Content-Type"] = "text/html; charset=utf-8";
		this->_body = body;
		int body_size = this->_body.length();
		this->_headers["Content-Length"] = SSTR(body_size);
		this->_response = this->construct_response();
		return ;
	}

	if (!_serv.get_location()[location].get_default_file().empty())
	{
		std::string path = _serv.get_location()[location].get_default_file();
		return (this->simple_response(200, "OK", path));
	}
	return (this->simple_response(200, "OK"));
}

void	HttpResponse::handle_post()
{
	std::string location = this->get_location();
	std::string upload_path = _serv.get_location()[location].get_upload_path();

	if (upload_path.empty())
		return (this->simple_response(405, "Uploading file cannot be performed on this location", this->_serv.get_error_pages()[405]));
	
	if (!is_directory(upload_path.c_str()))
		return (this->simple_response(500, "Internal Server Error", this->_serv.get_error_pages()[500]));

	std::string extension = get_file_ext(this->build_ressource_path());
	if (!ContentTypeList().count(extension))
		return (this->simple_response(415, "Unsupported Media Type", this->_serv.get_error_pages()[415]));

	std::string file_path = this->build_ressource_path();
	size_t pos = file_path.find_last_of('/');
	file_path = file_path.erase(0, pos + 1);
	file_path = upload_path + file_path;
	std::ofstream new_file(file_path.c_str());
	new_file << this->_req.get_body();
	new_file.close();

	this->_headers["Location"] = file_path;
	return (this->simple_response(201, "Created"));
}

void	HttpResponse::handle_delete(){

	std::string path = this->build_ressource_path();
	if (std::remove(path.c_str()) == 0)
		return (this->simple_response(204, "No Content"));
	else
		return (this->simple_response(500, "Internal Server Error", _serv.get_error_pages()[500]));
}