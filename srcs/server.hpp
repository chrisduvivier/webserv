#ifndef SELECT_SERV_HPP
# define SELECT_SERV_HPP

# include <list>
# include <string>
# include <iostream>
# include <unistd.h>
# include "lib.h"

# include <fcntl.h>

# include <sys/select.h>
# include <sys/socket.h>
# include <arpa/inet.h>

# define STDOUT 1
# define MAX_CLIENTS 1024

# define COLOR_RESET "\033[0m"
# define COLOR_BLUE "\033[1;34m"
# define COLOR_GREEN "\033[1;32m"

# define COLOR_BLUE_(str) COLOR_BLUE << str << COLOR_RESET
# define COLOR_GREEN_(str) COLOR_GREEN << str << COLOR_RESET

#endif