#include "Server.hpp"

Server::Server(std::vector<ServerConfig> conf, int domain, int service, int protocol) : _conf_vector(conf)
{
	std::string ip;
	int i = 0;
	int port;

	// iterate through serverconf vector to initialize sockets and push them in socket_vector
	for (std::vector<ServerConfig>::iterator iter_server = conf.begin(); iter_server != conf.end(); iter_server++)
	{
		ip = ip_to_string(iter_server->get_host());
		port = iter_server->get_port();
		/*std::cout << "Server number " << i << " : port is " << port << std::endl;
		std::cout << "Server number " << i << " : ip is " << ip << std::endl;*/
		Socket temp(port, ip, domain, service, protocol);
		_socket_vector.push_back(temp);
		try
		{
			init(temp.get_sock(), temp.get_address());
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			std::cerr << "server SETUP failed, exiting webserv...\n";
			exit(1);
		}
		i++;
	}
}

void Server::init(int sock, struct sockaddr_in address)
{
	// set socket as non blocking
	fcntl(sock, F_SETFL, O_NONBLOCK);
	if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw ServerException("Error Server: Error bind socket");
	if (listen(sock, BACKLOG_LEN) < 0)
		throw ServerException("Error Server: Failed to listen socket");
	for (int i = 0; i < FD_SETSIZE; i++)
		_awaiting_send[i] = false;
}

/*
**	Associate a configuration file to the server
*/
void Server::setConf(ServerConfig conf)
{
	this->_conf = conf;
}

/*
**	Read the request and create answer associated with the client socket number.
**	The result to e sent is store in a map `Cluster`
*/
void Server::handle_connection(int client_sock, ServerConfig conf)
{
	int ret = 0;
	char buffer[REQUEST_READ_BUFFER];
	HttpRequest client_request;
	std::string response;

	usleep(1000);
	bzero(&buffer, REQUEST_READ_BUFFER);
	ret = read(client_sock, buffer, REQUEST_READ_BUFFER);
	if (ret < 0 || ret == 0)
		throw ServerException("Exception: Couldn't read from client socket");
	client_request.parse_request(buffer);
	HttpResponse http_response(client_request, conf);
	http_response.build_response();
	response = http_response.get_response();

	// insert to map
	if (_cluster._response_queue.find(client_sock) == _cluster._response_queue.end()) // dont exists
	{
		// create vector with 1 string inside for initilaization
		std::vector<std::string> tmp;
		tmp.push_back(response);
		_cluster._response_queue.insert(std::pair<int, std::vector<std::string> >(client_sock, tmp));
	}
	else
		_cluster._response_queue.find(client_sock)->second.push_back(response);
}

/*
**	Find the response to the client (client_socket) from cluster and send it.
*/
int Server::send_response(int client_sock)
{
	if (_cluster._response_queue.find(client_sock) == _cluster._response_queue.end())
		return (0);
	std::string response = _cluster._response_queue.find(client_sock)->second.back();
	size_t response_size = response.length();

	int res = send(client_sock, response.c_str(), response_size, 0);
	if (res < 0 || res == 0)
		throw ServerException("Exception: Error send: failed to send response to client");
	_cluster._response_queue.find(client_sock)->second.pop_back(); // remove response only after successful send
	return (0);
}

void Server::remove_client(int i)
{
	std::vector<Socket>::iterator sock_iter;

	for (sock_iter = _socket_vector.begin(); sock_iter != _socket_vector.end(); sock_iter++)
	{
		if (std::find(sock_iter->_client.begin(), sock_iter->_client.end(), i) != sock_iter->_client.end()) // search what server the client is linked to
		{
			sock_iter->_client.erase(std::remove(sock_iter->_client.begin(), sock_iter->_client.end(), i), sock_iter->_client.end()); // remove client
			break;
		}
	}
}

void signalHandler(int i)
{
	std::cout << YELLOW << "\nInterrupt signal received.\nExiting webserv\n\n"
			  << RESET;
	close(i);
	exit(1);
}

/*
**
*/
int Server::run()
{
	fd_set current_sockets;
	fd_set read_sockets; // Also checked for new established connection
	fd_set write_sockets;
	bool new_connection = false;
	std::signal(SIGINT, signalHandler);
	std::vector<Socket>::iterator sock_iter = _socket_vector.begin();
	int send_answer = 0;
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

		struct timeval timeout; // for timeout
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;
		if (select(FD_SETSIZE, &read_sockets, &write_sockets, NULL, &timeout) < 0)
		{
			std::cout << "error: select error" << std::endl;
			return (-1);
		}
		for (int i = 0; i < FD_SETSIZE; i++) // TODO: possible improvement
		{
			if (_awaiting_send[i] == true && FD_ISSET(i, &write_sockets) && new_connection == false && send_answer == 1)
			{
				/*std::cout << "send" << std::endl;*/
				try
				{
					send_response(i);
				}
				catch (std::exception &e)
				{
					std::cout << e.what() << std::endl;
				}
				remove_client(i);
				_awaiting_send[i] = false;
				close(i);
				FD_CLR(i, &current_sockets);
				send_answer = 0;
				break ;
			}

			new_connection = false;			// reset
			if (FD_ISSET(i, &read_sockets)) // tests if fd i is in the set => ready for reading.
			{
				for (sock_iter = _socket_vector.begin(); sock_iter != _socket_vector.end(); sock_iter++)
				{
					if (i == sock_iter->get_sock()) // new connection is established
					{
						int client_socket = 0;
						/*usleep(100);*/
						/*std::cout << "accept" << std::endl;*/
						if ((client_socket = accept(i, NULL, NULL)) < 0)
						{
							std::cout << "error: Socket accept" << std::endl;
							/*close(i);
							remove_client(i);
							FD_CLR(i, &current_sockets);*/
							return (-1);
						}
						FD_SET(client_socket, &current_sockets);
						sock_iter->_client.push_back(client_socket); // remember the client so we can id which server is linked to
						new_connection = true;						 // signal to tell its a new connection
						send_answer = 0;
						break ;
					}
				}

				if (new_connection == false)
				{
					/*std::cout << "handle" << std::endl;*/
					if (FD_ISSET(i, &read_sockets) && _awaiting_send[i] == false) // read is ready
					{
						int server_num = 0; // find which server (in the list sock_iter->_client) contains the fd ready to be handled
						for (sock_iter = _socket_vector.begin(); sock_iter != _socket_vector.end(); sock_iter++)
						{
							if (std::find(sock_iter->_client.begin(), sock_iter->_client.end(), i) != sock_iter->_client.end()) // search what server the client is linked to
								break;
							server_num++;
						}
						try
						{
							handle_connection(i, _conf_vector[server_num]); // main function to handle requests
							_awaiting_send[i] = true;
							send_answer = 1;
						}
						catch (std::exception &e)
						{
							std::cout << e.what() << std::endl;
							remove_client(i);
							close(i); // remove client
							FD_CLR(i, &current_sockets);
							_awaiting_send[i] = false;
						}
						break ;
					}
				}
			}
		}
	}
	return (0); // should never quit
}