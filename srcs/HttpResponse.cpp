#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
{
	std::cout << "-- HttpResponse constructor called --" << std::endl;
}

HttpResponse::HttpResponse(HttpRequest request)
{
	std::string directory = "./html/";

	// assume at this point that the request is cleaned and error-free
	if (request.get_method() == "GET")
	{
		// get the path requested
		if (!request.get_url().empty())
		{
			std::string path = directory + request.get_url();

			
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
