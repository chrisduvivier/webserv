#include "SimpleSocket.hpp"

/* Constructor */
SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface)
{
	// define address structure
	bzero(&_address, sizeof(_address));	//zero out everything first
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(interface);

	// Establish socket
	_sock = socket(domain, service, protocol);
	test_connection(sock);

	// Establish network connection

}

SimpleSocket::test_connection(int domain)
{
	if (domain < 0)
	{
		perror("Failer to connect...");
		exit(EXIT_FAILURE);
	}
}

struct sockaddr_in  SimpleSocket::get_address(void) { return _address; }
int                 SimpleSocket::get_sock(void) { return _sock; }
int                 SimpleSocket::get_connection(void) { return connection; }