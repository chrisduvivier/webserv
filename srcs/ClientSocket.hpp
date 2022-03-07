#ifndef CLIENT_SOCKET_HPP
# define CLIENT_SOCKET_HPP

# include "SimpleSocket.hpp"

# define BUFFER_SIZE 1024

class ClientSocket : public SimpleSocket
{
	public:
		ClientSocket(int port, std::string ip, int domain = AF_INET, int service = SOCK_STREAM, int protocol = 0) : 
			SimpleSocket(port, ip, domain, service, protocol) {};
		
		// throw exception on error
		int		init();
		int		run();
};

#endif
