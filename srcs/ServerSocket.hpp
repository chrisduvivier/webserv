#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

# include "SimpleSocket.hpp"

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

# define REQUEST_READ_BUFFER  10256

class ServerSocket
{
	public:
		ServerSocket(std::vector<ServerConfig>	conf, int domain = AF_INET, int service = SOCK_STREAM, int protocol = 0);
		
		// throw exception on error
		void    			init(int sock, struct sockaddr_in address);
		int					run();
		void				setConf(ServerConfig conf);
		int					establish_socket(int domain, int service, int protocol);

	private:
		ServerConfig 				_conf;
		std::vector<ServerConfig>	_conf_vector;
		std::vector<SimpleSocket>	_socket_vector;

};

#endif
