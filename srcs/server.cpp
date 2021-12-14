#include "server.hpp"

static void        announce_client(int client_fd) {
    struct sockaddr_in  client_addr;
    int                 addr_len = sizeof(client_addr);

    if ((getsockname(client_fd, (struct sockaddr *) &client_addr,
                    (socklen_t *) &addr_len)) == -1)
	{
        std::cerr << "error: getsockname call" << std::endl;
        exit(1);
    }

    std::cout << COLOR_BLUE;
    std::cout << "New request from " << inet_ntoa(client_addr.sin_addr) << COLOR_RESET << std::endl;
}

static void        print_request(std::list<char *> request)
{
    std::cout << COLOR_GREEN;
    for (std::list<char *>::iterator it = request.begin(); it != request.end(); it++)
        std::cout << *it << std::endl;
    std::cout << COLOR_RESET;
}

static std::list<char *> request_reader(int client_fd) {
    char                        *buf;
    std::list<char *>           request;
    int                         status = 0;

    while ((status =
            get_next_line(client_fd, &buf)) > 0 && ft_strlen(buf) != 1) {
        request.push_back(buf);
    }
    if (status == -1) {
        std::cerr << "error: recv headers call : " << strerror(errno) \
            << std::endl;
        exit(1);
    }

    // get content-length
    std::list<char *>::iterator it = request.begin();
    char    content_key[] = "Content-length: ";
    while (it != request.end()
            && ft_strncmp(*it, content_key, 16) != 0) {
        it++;
    }
    if (it == request.end())  // return when no content-length is specified
        return (request);
    int content_len = ft_atoi(*it + 16);

    // body reading
    char    buf_2[content_len];
    if ((recv(client_fd, &buf_2, content_len, 0)) == -1) {
        std::cerr << "error: recv call : " << strerror(errno) \
            << std::endl;
        exit(1);
    }
    request.push_back(buf_2);
    return (request);
}

int listen_fd;

void        signal_handler(int signum)
{
    std::cout << "(" << signum << ") Server ending..." << std::endl;
    close(listen_fd);
    exit(1);
}

#define BACKLOG_LEN 10

void    print_usage(void) {
    std::cout << "select_server USAGE :" << std::endl;
    std::cout << "./select_server [PORT]" << std::endl;
    std::cout << "    [PORT] : number between 0-65535 is the port to listen. (don't use common port)" << std::endl;
}

void	clean_up(void) {}

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

 	/* Timeout specification */
    struct timeval	setTimeout;
	setTimeout.tv_sec = 1;
	setTimeout.tv_usec = 0;

	FD_ZERO(&current_sockets);
	while (true)
	{
		FD_SET(server_socket, &current_sockets);
		if (select(server_socket + 1, &current_sockets, NULL, NULL, &setTimeout) < 0)
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

	clean_up();

	return (0);	//should never quit
}

void PerformSelect(fd_set listeningSock, fd_set clients[], int iNumClients)
{
	/* Timeout specification */
    struct timeval	setTimeout;
	setTimeout.tv_sec = 1;
	setTimeout.tv_usec = 0;

	while (true)
	{
		FD_SET(listeningSock, &clients);
		if (select(listeningSock + 1, &clients, NULL, NULL, &setTimeout) < 0)
		{
			std::cerr << "error: select error" << std::endl;
			return (-1);
		}

		// check if listeningSock clients, in other word, that is ready for reading right now.
		if (FD_ISSET(listeningSock, &clients))
		{
			if ((client_fd = accept(listeningSock, NULL, NULL)) < 0)
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
		FD_CLR(listeningSock, &clients);
	}
}

fd_set m_ServerSock;            /* server's listening socket */
fd_set m_client_list[MAX_CLIENTS];
int    m_iNumclients;

void	initialize(int argc, char *argv[])
{
	struct sockaddr_in	serv_addr;

	if ( (m_ServerSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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

 	/* Timeout specification */
    struct timeval	setTimeout;
	setTimeout.tv_sec = 1;
	setTimeout.tv_usec = 0;

	FD_ZERO(&current_sockets);
}

int		main(int argc, char **argv)
{
    int     running = 1;  /* 1 if server should be running; 0 otherwise */

    /* process command-line arguments and initialize all variables  */
    /* also do sockets initialization								*/
    /* and create and start up the listening socket                 */
    initialize(argc, argv);

    while (running)
    {
		performSelect(m_ServerSock, m_client_list, m_client_list, m_iNumclients);
    } // end while running
        
 
    /*		close all open connections gracefully */
    /*		plus whatever other cleanup is needed.*/
    clean_up();
	return (0);
}