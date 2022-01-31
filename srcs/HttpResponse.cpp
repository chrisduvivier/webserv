#include "HttpResponse.hpp"

HttpResponse::HttpResponse() : 
	_protocol("HTTP/1.1")
{
	std::cout << "-- HttpResponse constructor called --" << std::endl;
}

HttpResponse::HttpResponse(HttpRequest request) : 
	_protocol("HTTP/1.1")
{
	std::string directory = "./html";

	std::cout << "method= " << request.get_method() << std::endl;

	// assume at this point that the request is cleaned and error-free
	if (request.get_method() == "GET")
	{
		// get the path requested
		if (!request.get_url().empty())
		{
			std::string path = directory + request.get_url();

			std::cout << "path = " << path << std::endl;

			// try to find the path, and if exists, open it.
			std::ifstream paramFile(path.c_str());
			if (paramFile)
			{
				std::stringstream buffer;
				buffer << paramFile.rdbuf();
				std::string body = buffer.str();
				std::cout << "body = " << body << std::endl;

				this->_status_code = 200;
				this->_status_text = "OK";
				this->_body = body;
			}
			else
			{
     			std::string error;
				std::stringstream tmp;
				tmp << "Error code: " << strerror(errno) << "File could not be opened!\n";
				error = tmp.str();
				throw MyException(error);
			}
		}
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

std::string	HttpResponse::construct_response()
{
	std::stringstream tmp;
	std::string response;

	tmp << this->_protocol << " " << this->_status_code << " " << this->_status_text << "\r\n\r\n" << this->_body;
	response = tmp.str();
	return (response);
}
