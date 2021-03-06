/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduvivie <cduvivie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 15:32:11 by ldavids           #+#    #+#             */
/*   Updated: 2022/03/14 16:52:14 by cduvivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/* Constructor */
Socket::Socket(int port, std::string ip, int domain, int service, int protocol)
{
	// zero out everything first
	bzero(&_address, sizeof(_address));

	// define address structure
	_address.sin_family = domain;
	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, ip.c_str(), &_address.sin_addr.s_addr) <= 0)
	{
		error_exit("Invalid address / Address not supported \n");
	}
	_address.sin_port = htons(port);

	// Establish socket
	_sock = establish_socket(domain, service, protocol);

	setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &_option_buffer, sizeof(_option_buffer));
}

int	Socket::establish_socket(int domain, int service, int protocol)
{
	int sock;
	if ( (sock = socket(domain, service, protocol)) < 0 )
	{
		std::cerr << "Error: Failed to create socket" << std::endl;
	}
	return (sock);
}

// void	Socket::set_connection(int connection_id) { _connection = connection_id; }

struct sockaddr_in  Socket::get_address(void) { return _address; }
int                 Socket::get_sock(void) { return _sock; }
int                 Socket::get_connection(void) { return _connection; }
