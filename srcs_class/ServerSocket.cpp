# include "ServerSocket.hpp"

void    ServerSocket::init()
{
	// set socket as non blocking
	fcntl(_sock, F_SETFL, O_NONBLOCK);

	if (bind(_sock, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		throw "Error: Error bind socket";
	}

	if (listen(_sock, BACKLOG_LEN) < 0)
	{
		throw "Error: Failed to listen socket";
	}
}

// HttpRequest handle_client_request(char *buffer, long len)
// {
// 	return (HttpRequest(buffer, len));
// }

// std::string response_to_client(std::string	)
// {

// }

int ServerSocket::run()
{
	int			client_sock;
	fd_set		working_sockets;
	std::string	response = "HTTP/1.1 200 OK\r\n\r\n Congratulation !";

	struct timeval	time;	//for timeout
	time.tv_sec = 1;
	time.tv_usec = 0;

	FD_ZERO(&working_sockets);
	while (true)
	{
		FD_SET(_sock, &working_sockets);
		if (select(_sock + 1, &working_sockets, NULL, NULL, &time) < 0)
		{
			std::cerr << "error: select error" << std::endl;
			return (-1);
		}

		// check if server_socket working_sockets, in other word, that is ready for reading right now.
		if (FD_ISSET(_sock, &working_sockets))
		{
			// TODO: is it ok to not pass 2nd 3rd arg?
			if ((client_sock = accept(_sock, NULL, NULL)) < 0)
			{
				std::cerr << "error: Socket accept" << std::endl;
				return (-1);
			}
			
			char buffer[30000] = {0};
        	long valread = read(client_sock , buffer, 30000);
        	std::cout << "Request from client: " << buffer << std::endl;

			// std::string	client_request = handle_client_request(buffer, valread);

			// std::string	response = response_to_client(client_request);

			// "send" with a zero flags argument is equivalent to write(2).
			if (send(client_sock, response.c_str(), strlen(response.c_str()) , 0) == -1)
			{
				std::cerr << "error: Response to client" << std::endl;
				return (-1);	
			}
			close(client_sock);
		}
		else {
			std::cout << "\n+++++++ Waiting for new connection ++++++++\n";
		}
		FD_CLR(_sock, &working_sockets);
	}
	return (0);	//should never quit
}