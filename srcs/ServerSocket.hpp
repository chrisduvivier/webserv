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
# include "MyException.hpp"

# define REQUEST_READ_BUFFER  10256

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
