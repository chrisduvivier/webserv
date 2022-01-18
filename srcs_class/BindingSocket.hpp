#ifndef BINDING_SOCKET_HPP
# define BINDING_SOCKET_HPP

# include "SimpleSocket.hpp"

class BindingSocket : public SimpleSocket
{
	public:
		
		BindingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(int domain, int service, int protocol, int port, u_long interface)

		void  test_connection(int);

		struct sockaddr_in  get_address(void);
		int                 get_sock(void);
		int                 get_connection(void);
	
	private:
		struct sockaddr_in  _address;
		int                 _sock;
		int                 _connection;
}



#endif