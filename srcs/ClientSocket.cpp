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
	std::string message_to_server = \
        "GET /www HTTP/1.1Host: localhost:8080\
        Connection: keep-alive\
        sec-ch-ua: \"Chromium\";v=\"88\", \"Google Chrome\";v=\"88\", \";Not A Brand\";v=\"99\"\
        sec-ch-ua-mobile: ?0\
        Upgrade-Insecure-Requests: 1\
        User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36\
        Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\
        Sec-Fetch-Site: none\
        Sec-Fetch-Mode: navigate\
        Sec-Fetch-User: ?1\
        Sec-Fetch-Dest: document\
        Accept-Encoding: gzip, deflate, br Accept-Language: en-US,en;q=0.9,ja;q=0.8,fr;q=0.7";

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