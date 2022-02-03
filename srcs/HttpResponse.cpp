#include "HttpResponse.hpp"

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
	tmp << "\r\n\r\n" << this->_body;

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
		throw MyException("Exception: Not currently supported\n");
	}
	else if (request.get_method() == "DELETE")
	{
		throw MyException("Exception: Not currently supported\n");
	}
	else
	{
		throw MyException("Exception: Unknown Method detected from request\n");
	}
}

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

void	HttpResponse::handle_get_request(HttpRequest request)
{
	// std::string directory = "./html";
	// // get the path requested
	// if (!request.get_url().empty())
	// {
	// 	std::string path = directory + request.get_url();

	// 	// std::cout << "path = " << path << std::endl;

	// 	// try to find the path, and if exists, open it.
	// 	std::ifstream paramFile(path.c_str());	//only accepts const * char so need to convert
	// 	if (paramFile)
	// 	{
	// 		std::stringstream buffer;
	// 		buffer << paramFile.rdbuf();		// from filestream to strstream 
	// 		std::string body = buffer.str();	// convert stream to string
	// 		// std::cout << "body = " << body << std::endl;

	// 		this->_status_code = 200;			// This part will handle the status code, text, and body 
	// 		this->_status_text = "OK";
	// 		this->_body = body;

	// 		// explicitly add length for now
	// 		this->_headers["Content-Length"] = body.length();
	// 	}
	// 	else
	// 	{
	// 		std::string error; //create error string using string stream
	// 		std::stringstream tmp;
	// 		tmp << "Error code: " << strerror(errno) << "File could not be opened!\n";
	// 		error = tmp.str();
	// 		throw MyException(error);
	// 	}
	// }

	/* probably going to be a server attribute */
	std::string directory = "./html";

	if (!request.get_url().empty())
	{
		/* we build the path to the ressource requested */
		std::string path = directory + request.get_url();

		/* we check if the ressource exists - Error 404 if it does not */
		std::ifstream ressource(path.c_str());
		if (!ressource)
		{
			this->_protocol = "HTTP/1.1";
			this->_status_code = 404;
			this->_status_text = "Not Found";
			this->_headers["Content-Type"] = "text/html; charset=utf-8";
			this->_body = "<!DOCTYPE html>\n<html>\n<title>404 Not Found</title>\n<body>\n<div>\n<H1>404 Not Found</H1>\n<p>Unable to find a representation of the requested resource</p>\n</div>\n</body>\n</html>";
			this->_headers["Content-Length"] = std::to_string(this->_body.length());
			return ;
		}
		this->_protocol = "HTTP/1.1";
		this->_status_code = 200;
		this->_status_text = "OK";
		this->_headers["Content-Type"] = ContentTypeList()[path.substr(path.find('.') + 1)];
		std::stringstream buff;
		buff << ressource.rdbuf();
		ressource.close();
		this->_body = buff.str();
		this->_headers["Content-Length"] = std::to_string(this->_body.length());
	}

}


