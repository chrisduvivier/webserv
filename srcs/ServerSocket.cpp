# include "ServerSocket.hpp"

ServerSocket::ServerSocket(std::vector<ServerConfig> conf, int domain, int service, int protocol)
{
	int i = 0;

	std::vector<ServerConfig>::iterator it1 = conf.begin();
	std::vector<ServerConfig>::iterator it2 = conf.end();
	_conf_vector = conf;
	std::string ip;
	int port;

	// iterate through serverconf vector to initialize sockets and push them in socket_vector
	while (it1 != it2) 
	{
		ip = ip_to_string(it1->get_host());
		port = it1->get_port();
		std::cout << "Server number " << i << " : port is " << port << std::endl;
		std::cout << "Server number " << i << " : ip is " << ip << std::endl;
		SimpleSocket temp(port, ip, domain, service, protocol);
		_socket_vector.push_back(temp);
		try {
      	  init(temp.get_sock(), temp.get_address());
   		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			exit(1);
		}
		it1++;
		i++;
	}
}

void    ServerSocket::init(int sock, struct sockaddr_in address)
{
	// set socket as non blocking
	fcntl(sock, F_SETFL, O_NONBLOCK);

	if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		throw MyException("Error: Error bind socket");
	}

	if (listen(sock, BACKLOG_LEN) < 0)
	{
		throw MyException("Error: Failed to listen socket");
	}
}

/*
	--
		Associate a configuration file to the server
	--
*/
void	ServerSocket::setConf(ServerConfig conf)
{
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
	int			y = 0;
	std::vector<SimpleSocket>::iterator it1 = _socket_vector.begin();
	std::vector<SimpleSocket>::iterator it2 = _socket_vector.end();

	// initialize my current set
	FD_ZERO(&current_sockets);
	while (it1 != it2)
	{
		FD_SET(it1->get_sock(), &current_sockets);
		std::cout << "Socket listening on port " << ntohs(it1->get_address().sin_port) << std::endl;
		it1++;
	}
	while (true)
	{
		ready_sockets = current_sockets;	//because select is destructive
		struct timeval	timeout;			//for timeout
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
			it1 = _socket_vector.begin();
			it2 = _socket_vector.end();	
			y = 0;
			// tests if the file descriptor `i` is part of the set => ready for reading right now.
			if (FD_ISSET(i, &ready_sockets))
			{
				while (it1 != it2)
				{
					y = 0;
					if (i == it1->get_sock())	//new connection is established
					{
						int client_socket;
						if ((client_socket = accept(i, NULL, NULL)) < 0)
						{
							std::cout << "error: Socket accept" << std::endl;
							return (-1);
						}
						FD_SET(client_socket, &current_sockets);
						it1->_client.push_back(client_socket); // remember the client so we can id which server is linked to
						y = 1; // signal to tell its a new connection 
						break ;
					}
					it1++;
				}
				if (y != 1)
				{
					it1 = _socket_vector.begin();
					it2 = _socket_vector.end();
					y = 0;
					while (it1 != it2)
					{
						if (std::find(it1->_client.begin(), it1->_client.end(), i) != it1->_client.end()) // search what server the client is linked to
							break ;
						y++;
						it1++;
					}
					try {
						handle_connection(i, _conf_vector[y]); // main function to handle requests
					} catch (std::exception &e) {
						std::cout << e.what() << std::endl;
					}
					it1->_client.erase(std::remove(it1->_client.begin(), it1->_client.end(), i), it1->_client.end()); // forget client
					close(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}
	return (0);	//should never quit
}