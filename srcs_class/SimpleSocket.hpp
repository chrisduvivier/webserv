#ifndef SIMPLE_SOCKET_HPP
# define SIMPLE_SOCKET_HPP

# include <string>
# include <iostream>
# include <unistd.h>
# include <fcntl.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <arpa/inet.h>

//	max number of pending connections that can be queued up before
//	connections are refused.
# define BACKLOG_LEN 3

class SimpleSocket
{
	public:
		//constructor
		SimpleSocket(int domain, int service, int protocol, int port, u_long interface);

		void	establish_socket(int domain, int service, int protocol);

		struct sockaddr_in  get_address(void);
		int                 get_sock(void);
		int                 get_connection(void);

		
	
	private:
		struct sockaddr_in  _address;
		int                 _sock;
		int                 _connection;
}



#endif