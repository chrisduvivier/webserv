#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> /* -- inet_addr function -- */
#include <netinet/in.h> /* -- AF_INET & SOCK_STREAM defines -- */
#include <errno.h>  /* --   Used to get the error value of socket's function          -- */
                    /* --   Most of socket's function return -1 in case of an error   -- */

#define SERVER_PORT 18000
#define MAXLINE 4096

int main(int argc, char **argv)
{
    /* --   A socket represent the link with a distant machine -- */
    /* --   It can be used to send and receive datas           -- */


    /* --   hton*() — Translate a 'machine' number into network byte order          -- */
    /* --   this function is an Host to network function, used to convert data from -- */
    /* --   the machine to data for the 'network'. Network communications are       -- */
    /* --   big-endian (AKA network byte order).                                    -- */

    /* --   example :
            short htons(short value);
            long htonl(long value);     -- */


    /* --   ntoh*() — Is the reverse function.          -- */
    /* --   It translates network datas into Host datas -- */

    /* --   example :
            short ntohs(short value);
            long ntohl(long value);     -- */

    /* --   int socket(int family, int type, int protocol); -- */
    /* --   family: adress family that can communicate with the socket, often AF_INET (IPv4) or AF_INET6 (IPv6)   -- */
    /* --   type: type of the socket. SOCK_STREAM for TCP, SOCK_DGRAM for UDP   -- */
    /* --   protocol: is a standard set of rules for transferring data. It depend on the family and type of the socket-- */
    /* --   0 is used by default, mostly used are IPPROTO_TCP and IPPROTO_UDP   -- */
    /* --   We have to close the socket after it's using, to free the port and ressources it used   -- */
    
    /* --   example:
            int sockfd = socket(AF_INET, SOCK_STREAM, 0); -- */


    /* --  int connect(int _socket, const struct sockaddr* server, socklen_t serverlen); -- */
    /* --  _socket: socket to connect  -- */
    /* --  server:  structure that represent the server to connect to   -- */
    /* --  serverlen: size of the server structure -- */
    /* --  The call to this function is blocking while the connection is not performed   -- */
    /* --  It means that if the function returns, the distant machine accepted and performed the connection (or it returns an error) -- */
    /* --  Once the socket connected, it can be used as an identifer to the distant machine -- */
    /* --  in function that requires a socket as parameter, to receive datas from or to send datas to this machine -- */
    
    /* --   example: 
            sockaddr_in server;
            server.sin_addr.s_addr = inet_addr(const char* ipaddress);
            server.sin_family = AF_INET;
            server.sin_port = htons(int port);
            if (connect(sockfd, &server, sizeof(server)) != 0)   -- */

    /* --   The inet_addr() function shall convert the string pointed to by cp, -- */
    /* --   in the standard IPv4 dotted decimal notation,  -- */
    /* --   to an integer value suitable for use as an Internet address. -- */

    if (argc != 2)
    {
        std::cout << "Error: enter an IP adress.";
        return (1);
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cout << "Error: cannot create the socket" << std::endl;
        return (1);
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(argv[1]); //convert a text representation of an address to a binary one
    // if (inet_pton(AF_INET, argv[1], &server.sin_addr) <= 0)
    // {
    //     std::cout << "Error: cannot convert IP adress correctly" << std::endl;
    //     return (-1);
    // }
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) != 0)
    {
        std::cout << "Error: failed to connect to the server" << std::endl;
        return (1);
    }
    std::cout << "Socket connection: success!" << std::endl;

    char    sendline[MAXLINE];
    char    recvline[MAXLINE];
    int     sendbytes;


    sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");

    // sprintf(sendline, "GET / HTTP/1.1\nHost: example.com\r\n\r\n");
    sendbytes = std::strlen(sendline);

    if (write(sockfd, sendline, sendbytes) != sendbytes) //by writing into the socket, we send sendline to the server
    {
        std::cout << "Error: cannot send the request to the server" << std::endl;
        return (1);
    }

    int n = 0;
    memset(recvline, 0, MAXLINE);
    while ( (n = read(sockfd, recvline, MAXLINE - 1)) > 0) //by reading the socket, we read the response from the server
    {
        std::cout << recvline;
    }
    if (n < 0)
    {
        std::cout << "Error: cannot read the response from the server" << std::endl;
    }


	// send(sockfd, sendline, sendbytes, 0);
	// recvline[recv(sockfd, recvline, MAXLINE - 1, 0)] = '\0';
	// std::cout << recvline;

    close(sockfd);

    return (0);
}