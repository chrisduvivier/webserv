#include "HttpRequest.hpp"

HttpRequest::HttpRequest(std::vector<std::string> client_request)
{
    std::cout << "--HttpRequest constructor called--" << std::endl;

    std::cout << "1st line: client_request[0]=" << client_request[0] << std::endl;
}