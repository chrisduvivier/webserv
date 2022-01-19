#include "SimpleSocket.hpp"

/* Constructor */
SimpleSocket::SimpleSocket(int port, u_long interface, int domain, int service, int protocol)
{
	// zero out everything first
	bzero(&_address, sizeof(_address));

	// define address structure
	_address.sin_family = domain;
	_address.sin_addr.s_addr = htonl(interface);
	_address.sin_port = htons(port);

	// Establish socket
	_sock = establish_socket(domain, service, protocol);
}

int	SimpleSocket::establish_socket(int domain, int service, int protocol)
{
	int sock;
	if ( (sock = socket(domain, service, protocol)) < 0 )
	{
		std::cerr << "Error: Failed to create socket" << std::endl;
	}
	return (sock);
}

// void	SimpleSocket::set_connection(int connection_id) { _connection = connection_id; }

struct sockaddr_in  SimpleSocket::get_address(void) { return _address; }
int                 SimpleSocket::get_sock(void) { return _sock; }
int                 SimpleSocket::get_connection(void) { return _connection; }
