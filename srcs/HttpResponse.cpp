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

void	HttpResponse::handle_get_request(HttpRequest request)
{
	std::string directory = "./html";
	// get the path requested
	if (!request.get_url().empty())
	{
		std::string path = directory + request.get_url();

		// std::cout << "path = " << path << std::endl;

		// try to find the path, and if exists, open it.
		std::ifstream paramFile(path.c_str());	//only accepts const * char so need to convert
		if (paramFile)
		{
			std::stringstream buffer;
			buffer << paramFile.rdbuf();		// from filestream to strstream 
			std::string body = buffer.str();	// convert stream to string
			// std::cout << "body = " << body << std::endl;

			this->_status_code = 200;			// This part will handle the status code, text, and body 
			this->_status_text = "OK";
			this->_body = body;

			// explicitly add length for now
			this->_headers["Content-Length"] = body.length();
		}
		else
		{
			std::string error; //create error string using string stream
			std::stringstream tmp;
			tmp << "Error code: " << strerror(errno) << "File could not be opened!\n";
			error = tmp.str();
			throw MyException(error);
		}
	}
}
