#ifndef LIB_H_
# define LIB_H_

# include <arpa/inet.h>
# include <sys/socket.h>
# include <signal.h>

typedef char t_bool;

# define FALSE 0
# define TRUE 1

int     ft_atoi(const char *str);
int     ft_strlen(char *str);
int     ft_strncmp(char *s1, char *s2, unsigned int n);

#endif