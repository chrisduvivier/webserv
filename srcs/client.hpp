#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <sys/socket.h>
# include <sys/types.h>
# include <signal.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <stdarg.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/time.h>
# include <sys/ioctl.h>
# include <netdb.h>
# include <stdlib.h>

// standard HTTP
# define SERVER_PORT 80

# define MAXLINE 4096
# define SA struct sockaddr

void    err_n_die(const char *fmt, ...);


#endif
