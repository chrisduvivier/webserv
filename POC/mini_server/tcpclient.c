#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

// standard HTTP
#define SERVER_PORT 80

#define MAXLINE 4096
#define SA struct sockaddr

void    err_n_die(const char *fmt, ...);

int     main(int argc, char **argv)
{
	int                 sockfd, n;
	int                 sendbytes;
	struct sockaddr_in  servaddr;
	char                sendline[MAXLINE];
	char                recvline[MAXLINE];

	// usage check
	if (argc != 2)
		err_n_die("usage: %s <server address>", argv[0]);

	// create a socket (= basically like a FILE with 2 way connection: you can write/read data into/out of.)
	if (  (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_n_die("Error while creating the socket");

	//zero out the address
	bzero(&servaddr, sizeof(servaddr));

	// specify address family  
	servaddr.sin_family = AF_INET;
	// Port (host to network, short -> convert to network standard bite order)
	servaddr.sin_port = htons(SERVER_PORT);	/* char server */

	// convert text to binary ["1", "2", "3", "4"] -> [1,2,3,4]
	if (inet_pton( AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_n_die("inet_pton error for %s ", argv[1]);

	// connect to server
	if ( connect( sockfd, (SA *)  &servaddr, sizeof(servaddr)) < 0 )
		err_n_die( "connect failed!" );

	// create a line to send.
	sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");	 // "i want a page"
	sendbytes = strlen(sendline);

	//Send the request
	if ( write(sockfd, sendline, sendbytes ) != sendbytes )
		err_n_die("write_error");

	memset(recvline, 0, MAXLINE);

	// Now read the server response
	while ( (n = read(sockfd, recvline, MAXLINE-1)) > 0)
	{
		printf("%s", recvline);
		memset(recvline, 0, MAXLINE);
	}
	if (n < 0)
		err_n_die("read error");

	exit(0);
}

void    err_n_die(const char *fmt, ...)
{
	int errno_save;
	va_list     ap;

	errno_save = errno;

	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	fflush(stdout);

	if (errno_save != 0)
	{
		fprintf(stdout, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
		fprintf(stdout, "\n");
		fflush(stdout);
	}
	va_end(ap);
	exit(1);
}