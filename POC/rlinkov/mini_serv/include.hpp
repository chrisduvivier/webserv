#ifndef INCLUDE_HPP
#define INCLUDE_HPP

#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>


#define SERVER_PORT 18000 //usually 80, here another port to dodge any common error
#define MAXLINE	4096
#define SA struct sockaddr

#endif