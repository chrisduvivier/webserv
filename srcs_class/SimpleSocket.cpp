#include "SimpleSocket.hpp"

/* Constructor */
SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface)
{
	// define address structure
	bzero(&_address, sizeof(_address));	//zero out everything first
	_address.sin_family = domain;
	_address.sin_addr.s_addr = htonl(interface);
	_address.sin_port = htons(port);

	// Establish socket
	_sock = establish_socket(domain, service, protocol);

	if (bind(_sock, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		std::cerr << "Error: Error bind socket" << std::endl;
		return (EXIT_FAILURE);	
	}

	if (listen(_sock, BACKLOG_LEN) < 0)
	{
		std::cerr << "Error: Failed to listen socket" << std::endl;
		return (EXIT_FAILURE);
	}

	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
	{
		perror("In accept");            
		exit(EXIT_FAILURE);        
	}
}

void	SimpleSocket::establish_socket(int domain, int service, int protocol)
{
	if ( (_sock = socket(domain, service, protocol)) < 0 )
	{
		std::cerr << "Error: Failed to create socket" << std::endl;
		return (EXIT_FAILURE);
	}
}

void	SimpleSocket::set_connection(int connection_id) { _connection = connection_id; }

struct sockaddr_in  SimpleSocket::get_address(void) { return _address; }
int                 SimpleSocket::get_sock(void) { return _sock; }
int                 SimpleSocket::get_connection(void) { return connection; }