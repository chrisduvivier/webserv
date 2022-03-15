/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduvivie <cduvivie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 15:10:00 by cduvivie          #+#    #+#             */
/*   Updated: 2022/03/15 15:10:24 by cduvivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Socket.hpp"

// class HttpRequest;
// class HttpResponse;

/* According to POSIX.1-2001 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

# include <string>
# include <sstream>
# include <fcntl.h>
# include <vector>

# include "utils.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "ServerException.hpp"

# include "ServerConfig.hpp"
# include "Cluster.hpp"

# define REQUEST_READ_BUFFER  10256

class Server
{
	public:
		Server(std::vector<ServerConfig>	conf, int domain = AF_INET, int service = SOCK_STREAM, int protocol = 0);
		
		// throw exception on error
		void    			init(int sock, struct sockaddr_in address);
		int					run();
		void				setConf(ServerConfig conf);
		int					establish_socket(int domain, int service, int protocol);
		void				handle_connection(int client_sock, ServerConfig conf);
		int					send_response(int client_sock);
		void				remove_client(int i);

	private:
		ServerConfig 				_conf;
		std::vector<ServerConfig>	_conf_vector;
		std::vector<Socket>	_socket_vector;
		Cluster						_cluster;
};

#endif
