/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduvivie <cduvivie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 15:32:29 by ldavids           #+#    #+#             */
/*   Updated: 2022/03/14 16:50:55 by cduvivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <string>
# include <iostream>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include "utils.hpp"

//	max number of pending connections that can be queued up before
//	connections are refused.
# define BACKLOG_LEN 3

class Socket
{
	public:
		//constructor
		Socket(int port, std::string ip, int domain, int service, int protocol);

		int	establish_socket(int domain, int service, int protocol);

		struct sockaddr_in  get_address(void);
		int                 get_sock(void);
		int                 get_connection(void);
		std::vector<int>	_client;

	protected:
		struct sockaddr_in  _address;
		int                 _sock;
		int                 _connection;
		int                 _option_buffer;
};

#endif
