#include "select_serv.hpp"

#define BACKLOG_LEN 10


void    print_usage(void) {
    std::cout << "select_server USAGE :" << std::endl;
    std::cout << "./select_server [PORT]" << std::endl;
    std::cout << "    [PORT] : number between 0-65535 is the port to listen. (don't use common port)" << std::endl;
}

int     select_server(int port)
{
	int					server_socket;
	int					client_fd;
	struct sockaddr_in	serv_addr;

	if ( (server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "error: Socket creation" << std::endl;
		return (-1);
	}

	bzero(&serv_addr, sizeof(serv_addr));	//zero out everything first
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	serv_addr.sin_family = AF_INET;

	fcntl(server_socket, F_SETFL, O_NONBLOCK);

	if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "error: Socket bind" << std::endl;
		return (-1);	
	}


	if (listen(server_socket, BACKLOG_LEN) < 0)
	{
		std::cerr << "error: Socket listen" << std::endl;
		return (-1);	
	}

	// init fd_set
	fd_set		current_sockets;
	char		response[] = "HTTP/1.1 200 OK\r\n\r\n Congratulation !";

    struct timeval	time;	//for timeout
	time.tv_sec = 1;
	time.tv_usec = 0;

	FD_ZERO(&current_sockets);
	while (true)
	{
		FD_SET(server_socket, &current_sockets);
		if (select(server_socket + 1, &current_sockets, NULL, NULL, &time) < 0)
		{
			std::cerr << "error: select error" << std::endl;
			return (-1);
		}

		// check if server_socket current_sockets, in other word, that is ready for reading right now.
		if (FD_ISSET(server_socket, &current_sockets))
		{
			if ((client_fd = accept(server_socket, NULL, NULL)) < 0)
			{
				std::cerr << "error: Socket accept" << std::endl;
				return (-1);	
			}
			
			// print log
			std::cout << "Toc toc ! Client here !" << std::endl;
			
			if (send(client_fd, response, strlen(response) , 0 ) == -1)
			{
				std::cerr << "error: Response to client" << std::endl;
				return -1;	
			}
			close(client_fd);
		}
		else {
			std::cout << "Waiting for connections...\n";
		}
		FD_CLR(server_socket, &current_sockets);
	}
	return (0);	//should never quit
}

int        main(int argc, char **argv) {
    if (argc != 2) {
        print_usage();
        return (1);
    }
    select_server(ft_atoi(argv[1]));
    return (0);
}
