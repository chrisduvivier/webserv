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
		throw ServerException("Error: Error bind socket");
	}

	if (listen(sock, BACKLOG_LEN) < 0)
	{
		throw ServerException("Error: Failed to listen socket");
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
int	ServerSocket::handle_connection(int client_sock, ServerConfig conf)
{
	int			ret = 0;
	char		buffer[REQUEST_READ_BUFFER];
	HttpRequest client_request;
	std::string	response;

	bzero(&buffer, REQUEST_READ_BUFFER);
	ret = read(client_sock, buffer, REQUEST_READ_BUFFER);
	if (ret < 0)
		throw ServerException("Exception: Couldn't read from client socket");

	client_request.parse_request(buffer);
	HttpResponse http_response(client_request, conf);
	http_response.build_response();
	response = http_response.get_response();
	
	// insert to map
	if (_cluster._response_queue.find(client_sock) == _cluster._response_queue.end())	//dont exists
	{
		//create vector with 1 string inside for initilaization
		std::vector<std::string> tmp;
		tmp.push_back(response);
		_cluster._response_queue.insert(std::pair<int, std::vector<std::string> >(client_sock, tmp));
	}
	else
		_cluster._response_queue.find(client_sock)->second.push_back(response);
	return (0);
}


int	ServerSocket::send_response(int client_sock)
{	
	if (_cluster._response_queue.find(client_sock) == _cluster._response_queue.end())
		return (0);
	std::string response = _cluster._response_queue.find(client_sock)->second.back();
	_cluster._response_queue.find(client_sock)->second.pop_back();							//remove from vector
	size_t response_size = response.length();
	
	int			bytes_sent;
	const char 	*bytes_to_send = static_cast<const char *>(response.c_str());
	while (response_size > 0)
	{
		bytes_sent = send(client_sock, bytes_to_send, response_size, 0);
		if (bytes_sent < 0)
			throw ServerException("Exception: Error while sending response to client");
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
	fd_set		read_sockets;		// Also checked for new established connection
	fd_set		write_sockets;
	bool		new_connection = false;
	std::vector<SimpleSocket>::iterator sock_iter = _socket_vector.begin();

	// initialize my current set
	FD_ZERO(&current_sockets);
	while (sock_iter != _socket_vector.end())
	{
		FD_SET(sock_iter->get_sock(), &current_sockets);
		DEBUG("Socket listening on port " + SSTR(ntohs(sock_iter->get_address().sin_port)));
		sock_iter++;
	}
	DEBUG("+++++++ Server is Listening ++++++++");
	while (true)
	{
		read_sockets = current_sockets;
		write_sockets = current_sockets;

		struct timeval	timeout;			//for timeout
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;
		if (select(FD_SETSIZE, &read_sockets, &write_sockets, NULL, &timeout) < 0)
		{
			std::cout << "error: select error" << std::endl;
			return (-1);
		}
		for (int i=0; i < FD_SETSIZE; i++)		//TODO: possible improvement
		{
			new_connection = false;									//reset
			if (FD_ISSET(i, &read_sockets))							//tests if fd i is in the set => ready for reading.
			{
				for (sock_iter = _socket_vector.begin(); sock_iter != _socket_vector.end(); sock_iter++)
				{
					if (i == sock_iter->get_sock())						//new connection is established
					{
						int client_socket;
						if ((client_socket = accept(i, NULL, NULL)) < 0)
						{
							std::cout << "error: Socket accept" << std::endl;
							return (-1);
						}
						FD_SET(client_socket, &current_sockets);
						sock_iter->_client.push_back(client_socket);	// remember the client so we can id which server is linked to
						new_connection = true; 								// signal to tell its a new connection 
						break ;
					}
				}
	
				if (new_connection == false)
				{
					//check if read is ready
					if (FD_ISSET(i, &read_sockets))
					{
						int server_num = 0;					// find which server (in the list sock_iter->_client) contains the fd ready to be handled
						for (sock_iter = _socket_vector.begin(); sock_iter != _socket_vector.end(); sock_iter++)
						{
							if (std::find(sock_iter->_client.begin(), sock_iter->_client.end(), i) != sock_iter->_client.end()) // search what server the client is linked to
								break ;
							server_num++;
						}
						try {
							handle_connection(i, _conf_vector[server_num]); // main function to handle requests
						} catch (std::exception &e) {
							std::cout << e.what() << std::endl;
						}
					}

					if (FD_ISSET(i, &write_sockets))
					{
						try {
							send_response(i);
							for (sock_iter = _socket_vector.begin(); sock_iter != _socket_vector.end(); sock_iter++)
							{
								if (std::find(sock_iter->_client.begin(), sock_iter->_client.end(), i) != sock_iter->_client.end()) // search what server the client is linked to
								{
									sock_iter->_client.erase(std::remove(sock_iter->_client.begin(), sock_iter->_client.end(), i), sock_iter->_client.end()); //remove client
									break ;
								}
							}
						} catch (std::exception &e) {
							std::cout << e.what() << std::endl;
						}
					}

					close(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}

		
	}
	return (0);	//should never quit
}