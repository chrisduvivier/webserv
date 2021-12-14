#include "include.hpp"

#include <string.h>

int	main(int argc, char **argv)
{
	(void)argv;
	(void)argc;

	int					listenfd, connfd, n;
	struct sockaddr_in	servaddr;
	uint8_t				buff[MAXLINE + 1];
	uint8_t				recvline[MAXLINE + 1];

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Error: listen socket creation failed" << std::endl;
		return (1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family 		= AF_INET;			  //family of adresses we're responding to
	servaddr.sin_addr.s_addr 	= htonl(INADDR_ANY);  //INTERNET ADRESS ANY -> response to any types of internet adresses
	servaddr.sin_port 			= htons(SERVER_PORT); //port we're listening on

	
	/* now that our adress is setup */

	/* bind: take a socket and specify the adress he's listening to */
	if ((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0)
	{
		std::cout << "Error: cannot bind the socket to the listen port" << std::endl;
		return (1);
	}

	/* listen: listening on that socket */
	if ((listen(listenfd, 10)) < 0)
	{
		std::cout << "Error: cannot listen to the port" << std::endl;
		return (1);
	}

	//infinite loop that accept and handle connection until you no longer want that server
	for( ; ; ){
		//struct sockaddr_in 	addr;
		//socklen_t			addr_len;

		//accept blocks until an incoming connection arrives
		//it returns a "file descriptor" to the connection
		std::cout << "waiting for a connection on port " << SERVER_PORT << std::endl;
		fflush(stdout);
		connfd = accept(listenfd, (SA *) NULL, NULL); //last 2 arguments allow you to get the address of who's connected
		//connfd is the socket that will be used to talk with the client, it waits until someone connects

		//zero out the receive buffer to make sure it ends up null terminated
		memset(recvline, 0, MAXLINE);
		
		//Now, read the client's message
		while ( (n = read(connfd, recvline, MAXLINE - 1) ) > 0)
		{
			std::cout << recvline;

			if (recvline[n - 1] == '\n')
				break;

			memset(recvline, 0, MAXLINE);
		}

		if (n < 0)
		{
			std::cout << "Error: cannot read the client's message properly" << std::endl;
			return (1);
		}

		//now send a response
		snprintf((char*)buff, sizeof(buff), "HTTP/1.1 200 OK\r\n\r\nHello");

		//normally we want to check the results from write and close 
		//in case an error occurs, for now we ignore them
		write(connfd, (char*)buff, strlen((char*)buff));
		close(connfd);
	}

	return (0);
}