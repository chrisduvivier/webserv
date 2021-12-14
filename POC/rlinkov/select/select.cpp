#include "../mini_serv/include.hpp"

#include <string.h>

int	main(int argc, char **argv)
{
	(void)argv;
	(void)argc;

	int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	uint8_t				buff[MAXLINE + 1];

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Error: listen socket creation failed" << std::endl;
		return (1);
	}
	/* we now modify the fd to a non-blocking one */
	fcntl(listenfd, F_SETFL, O_NONBLOCK); // F_SETFL: set fd flag
	/* --
		If the O_NONBLOCK flag is not enabled, then the
		system call is blocked until the lock is removed or converted to
		a mode that is compatible with the access.  If the O_NONBLOCK
		flag is enabled, then the system call fails with the error
		EAGAIN.
	-- */

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family 		= AF_INET;			
	servaddr.sin_addr.s_addr 	= htonl(INADDR_ANY);
	servaddr.sin_port 			= htons(SERVER_PORT);

	if ((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0)
	{
		std::cout << "Error: cannot bind the socket to the listen port" << std::endl;
		return (1);
	}

	if ((listen(listenfd, 10)) < 0)
	{
		std::cout << "Error: cannot listen to the port" << std::endl;
		return (1);
	}

	//init fd_set
	fd_set	set;

	struct timeval	time;
	time.tv_sec = 1;
	time.tv_usec = 0;
	FD_ZERO(&set); //This macro initializes the file descriptor set "set" to be the empty set.
	
	for( ; ; )
	{
		FD_SET(listenfd, &set); //This macro adds listenfd to the file descriptor set "set"

		//int select (int nfds, fd_set *read-fds, fd_set *write-fds, fd_set *except-fds, struct timeval *timeout)
		select(FD_SETSIZE, &set, NULL, NULL, &time);
		/* --- 
		The select function blocks the calling process 
		until there is activity on any of the specified sets of file descriptors, 
		or until the timeout period has expired.
		--- */

		//This macro returns a nonzero value (true) if filedes is a member of the file descriptor set set, and zero (false) otherwise.
		if (FD_ISSET(listenfd, &set))
		{
			if ((connfd = accept(listenfd, NULL, NULL)) < 0)
			{
				std::cout << "Error: cannot accept the socket" << std::endl;
				return (1);
			}
			std::cout << "Hello, a client connect with a request !" << std::endl;
			snprintf((char*)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello\r\n\r\n");

			//normally we want to check the results from write and close 
			//in case an error occurs, for now we ignore them
			//write(connfd, (char*)buff, strlen((char*)buff));
			send(connfd, (char*)buff, strlen((char*)buff), 0);
			close(connfd);
		}
		else
		{
			std::cout << "No client trying to connect" << std::endl;
		}
		FD_CLR(listenfd, &set); //This macro removes filedes from the file descriptor set set.
	}

	return (0);
}