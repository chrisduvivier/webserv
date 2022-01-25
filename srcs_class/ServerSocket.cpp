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

HttpRequest handle_client_request(std::vector<std::string> client_request)
{
	HttpRequest request(client_request);
	return (request);
}

// std::string response_to_client(std::string	)
// {

// }

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
			

			// read_from_socket(client_sock);
			
			/* 
			**	Instead of processing HTTPrequest one line at a time, create huge string from the retunred lines,
			**	and process later. (TODO: check if need to adjust this function) 
			*/
			int		ret = 0;
			std::vector<std::string> client_request;
			char 	*tmp_line = NULL;
			while (((ret = get_next_line(client_sock, &tmp_line)) >= 0) && ft_strlen(tmp_line) > 0)
			{
				// store the line
				std::string line(tmp_line);
				client_request.push_back(line);

				free(tmp_line);
				tmp_line = NULL;

				// check if end of HTTP request via \r\n??????

				if (ret == 0)
					break;
			}
			if (ret < 0)
			{
				std::cerr << "error: Couldn't read from client socket" << std::endl;
				return (-1);
			}
        	std::cout << "Request from client accepted" << std::endl;
			handle_client_request(client_request);

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