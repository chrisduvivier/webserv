#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

# include "SimpleSocket.hpp"

# include <fcntl.h>
# include <sys/select.h>
# include <sys/time.h>

# include <vector>

# include "HttpRequest.hpp"

class ServerSocket : public SimpleSocket
{
	public:
		ServerSocket(int port, u_long interface = INADDR_ANY, int domain = AF_INET, int service = SOCK_STREAM, int protocol = 0) : 
			SimpleSocket(port, interface, domain, service, protocol) {};
		
		// throw exception on error
		void    init();
		int		run();
};

#endif
