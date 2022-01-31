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

/* 
**	
*/
int ServerSocket::run()
{
	int			client_sock;
	fd_set		working_sockets;
	std::string	response = "HTTP/1.1 200 OK\r\n\r\n Congratulation !";

	FD_ZERO(&working_sockets);
	while (true)
	{
		FD_SET(_sock, &working_sockets);

		struct timeval	timeout;	//for timeout
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		if (select(_sock + 1, &working_sockets, NULL, NULL, &timeout) < 0)
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

			HttpRequest client_http_request;
			try {
				client_http_request = parse_http_request(client_sock);
			} catch (std::exception& e) {
				std::cout << e.what() << std::endl;
				close(client_sock);
				continue ;				//close current socket and restart listening 
			}
			
			try {
				HttpResponse http_reponse(client_http_request);
				response = http_reponse.construct_response();
			} catch (std::exception &e) {
				std::cout << e.what() << std::endl;
				close(client_sock);
				continue ;
			}


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