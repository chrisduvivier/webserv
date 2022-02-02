# include "ServerSocket.hpp"

void    ServerSocket::init()
{
	// set socket as non blocking
	fcntl(_sock, F_SETFL, O_NONBLOCK);

	if (bind(_sock, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		throw MyException("Error: Error bind socket");
	}

	if (listen(_sock, BACKLOG_LEN) < 0)
	{
		throw MyException("Error: Failed to listen socket");
	}
}

/* 
**	Read the HTTPrequest one line at a time and create HttpRequest class obj.
*/
HttpRequest	parse_http_request(int client_sock)
{
	int		ret = 0;
	char	buffer[REQUEST_READ_BUFFER] = {0};

	ret = read(client_sock, buffer, REQUEST_READ_BUFFER);
	if (ret < 0)
		throw MyException("Exception: Couldn't read from client socket");
	printf("%s\n", buffer);

	HttpRequest client_request(buffer);
	return (client_request);
}

int	handle_connection(int client_sock)
{
	std::string	response = "HTTP/1.1 200 OK\r\n\r\n Congratulation !";

	HttpRequest client_http_request;
	try {
		client_http_request = parse_http_request(client_sock);
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		close(client_sock);
		return (-1);
	}
	
	try {
		HttpResponse http_reponse(client_http_request);
		response = http_reponse.construct_response();
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		close(client_sock);
		return (-1);
	}

	// "send" with a zero flags argument is equivalent to write(2).
	if (send(client_sock, response.c_str(), strlen(response.c_str()) , 0) == -1)
	{
		std::cerr << "error: Response to client" << std::endl;
		return (-1);
	}
	return (0);
}

/*
**
*/
int ServerSocket::run()
{
	// int			client_sock;
	fd_set		current_sockets, ready_sockets;
	// std::string	response = "HTTP/1.1 200 OK\r\n\r\n Congratulation !";

	// initialize my current set
	FD_ZERO(&current_sockets);
	FD_SET(this->_sock, &current_sockets);

	int max_socket_so_far = this->_sock; 
	while (true)
	{
		ready_sockets = current_sockets;	//because select is destructive

		struct timeval	timeout;	//for timeout
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, &timeout) < 0)
		{
			std::cout << "error: select error" << std::endl;
			return (-1);
		}

		for (int i=0; i < FD_SETSIZE; i++)
		{
			// check if server_socket current_sockets, in other word, that is ready for reading right now.
			if (FD_ISSET(i, &ready_sockets))
			{
				if (i == this->_sock)	//new connection is established
				{
					int client_socket;
					if ((client_socket = accept(_sock, NULL, NULL)) < 0)
					{
						std::cout << "error: Socket accept" << std::endl;
						return (-1);
					}
					FD_SET(client_socket, &current_sockets);
					// if (client_socket > max_socket_so_far)	//update current max
					// 	max_socket_so_far = client_socket;
				}
				else
				{
					handle_connection(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n";
	}
	return (0);	//should never quit
}