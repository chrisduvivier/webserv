#ifndef SELECT_SERV_HPP
# define SELECT_SERV_HPP

# include <list>
# include <string>
# include <iostream>
# include <unistd.h>

# include <fcntl.h>

# include <sys/select.h>
# include <sys/socket.h>
# include <arpa/inet.h>

# define STDOUT 1
# define MAX_CLIENTS 1024
# define BACKLOG_LEN 10         // TO DEFINE PROPERLY, JUST ARBITRARY VALUE IS CHOSEN NOW

# define COLOR_RESET "\033[0m"
# define COLOR_BLUE "\033[1;34m"
# define COLOR_GREEN "\033[1;32m"

# define COLOR_BLUE_(str) COLOR_BLUE << str << COLOR_RESET
# define COLOR_GREEN_(str) COLOR_GREEN << str << COLOR_RESET

class Server
{
	public:
		Server() {};
		
		~Server() {};

		/* handles socket creation and server address configuration, create and start listening socket */
		int initialize(int port)
		{
			if ((this->_serv_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			{
				std::cerr << "error: Socket creation" << std::endl;
				return (-1);
			}

			bzero(&this->_serv_addr, sizeof(this->_serv_addr));	//zero out everything first
			this->_serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
			this->_serv_addr.sin_port = htons(port);
			this->_serv_addr.sin_family = AF_INET;

			fcntl(_serv_socket, F_SETFL, O_NONBLOCK);

			if (bind(_serv_socket, (struct sockaddr *)&_serv_addr, sizeof(_serv_addr)) < 0)
			{
				std::cerr << "error: Socket bind" << std::endl;
				return (-1);	
			}

			if (listen(_serv_socket, BACKLOG_LEN) < 0)
			{
				std::cerr << "error: Socket listen" << std::endl;
				return (-1);	
			}
			return (0);
		};

		int performSelect()
		{
			int			client_fd;
			fd_set		working_sockets;
			char		response[] = "HTTP/1.1 200 OK\r\n\r\n Congratulation !";

			struct timeval	time;	//for timeout
			time.tv_sec = 1;
			time.tv_usec = 0;

			FD_ZERO(&working_sockets);
			while (true)
			{
				FD_SET(_serv_socket, &working_sockets);
				if (select(_serv_socket + 1, &working_sockets, NULL, NULL, &time) < 0)
				{
					std::cerr << "error: select error" << std::endl;
					return (-1);
				}

				// check if server_socket working_sockets, in other word, that is ready for reading right now.
				if (FD_ISSET(_serv_socket, &working_sockets))
				{
					if ((client_fd = accept(_serv_socket, NULL, NULL)) < 0)
					{
						std::cerr << "error: Socket accept" << std::endl;
						return (-1);	
					}
					
					// print log
					std::cout << "Toc toc ! Client here !" << std::endl;
					
					if (send(client_fd, get_http.c_str(), strlen(get_http.c_str()) , 0 ) == -1)
					{
						std::cerr << "error: Response to client" << std::endl;
						return -1;	
					}
					close(client_fd);
				}
				else {
					std::cout << "Waiting for connections...\n";
				}
				FD_CLR(_serv_socket, &working_sockets);
			}
			return (0);	//should never quit
		}
	
	private:
		int					_serv_socket;
		struct sockaddr_in	_serv_addr;
};

#endif