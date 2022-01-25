# include "ClientSocket.hpp"

int	ClientSocket::init()
{
	
	bzero(&_address.sin_addr, sizeof(_address.sin_addr));

	// Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &_address.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return (-1);
    }
	return (0);
}

int ClientSocket::run()
{
	std::string message_to_server = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	char buffer[BUFFER_SIZE] = "";
	long valread;

	if (connect(_sock, (struct sockaddr *)&_address, sizeof(_address)) < 0)
    {
		std::cerr << "\nConnection Failed \n";
        return (-1);
    }
    send(_sock , message_to_server.c_str(), strlen(message_to_server.c_str()) , 0);

	std::cout << "message sent to server\n";

    valread = read(_sock, buffer, BUFFER_SIZE);

    std::cout << "Response from server: " << buffer << std::endl;
	
    return (0);
}