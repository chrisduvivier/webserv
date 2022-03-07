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
	--
		Associate a configuration file to the server
	--
*/
void	ServerSocket::setConf(ServerConfig conf) {

	this->_conf = conf;
}

/*
**	Read the request and handle appropriate result
**  "send" with a zero flags argument is equivalent to write(2).
**	send doesn't always send all the bytes requested, it has to be used in a loop to ensure that all the data is correctly sent.
*/
int	handle_connection(int client_sock, ServerConfig conf)
{
	int			ret = 0;
	static int	request_number = 0;
	char		buffer[REQUEST_READ_BUFFER];
	std::string	response;

	bzero(&buffer, REQUEST_READ_BUFFER);
	ret = read(client_sock, buffer, REQUEST_READ_BUFFER);
	if (ret < 0)
		throw MyException("Exception: Couldn't read from client socket");
	
	// TODO: ONLY TO KEEP TRACK OF CLIENT REQUEST. DELETE AT END
	std::ofstream outfile;
	outfile.open("request_log.txt", std::ios_base::app); // append instead of overwrite
	std::stringstream tmp_request_number;
	tmp_request_number << request_number;
	outfile << "[" << tmp_request_number.str() << "]:";
	request_number++;
	outfile << std::string(buffer);
	// UNTIL HERE

	HttpRequest client_http_request(buffer);
	HttpResponse http_response(client_http_request, conf);
	http_response.build_response();
	response = http_response.get_response();

	size_t 		response_size = response.length();
	int			bytes_sent;
	const char 	*bytes_to_send = static_cast<const char *>(response.c_str());

	while (response_size > 0)
	{
		bytes_sent = send(client_sock, bytes_to_send, response_size, 0);
		if (bytes_sent < 0)
			throw MyException("Exception: Error while sending response to client");
		bytes_to_send += bytes_sent;
		response_size -= bytes_sent;
	}
	return (0);
}

/*
**
*/
int ServerSocket::run()
{
	fd_set		current_sockets;
	fd_set		ready_sockets;

	// initialize my current set
	FD_ZERO(&current_sockets);
	FD_SET(this->_sock, &current_sockets);

	std::cout << "Socket listening on port " << ntohs(this->get_address().sin_port) << std::endl;
	while (true)
	{
		ready_sockets = current_sockets;	//because select is destructive

		struct timeval	timeout;	//for timeout
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, &timeout) < 0)
		{
			std::cout << "error: select error" << std::endl;
			return (-1);
		}

		std::cout << "+++++++ Waiting for new connection ++++++++\n";
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
				}
				else
				{
					try {
						handle_connection(i, _conf);
					} catch (std::exception &e) {
						std::cout << e.what() << std::endl;
						close(i);
						FD_CLR(i, &current_sockets);
						break;
					}
					close(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}
	return (0);	//should never quit
}