#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define GNL_LINE_READED 1
# define GNL_EOF 0
# define GNL_ERROR -1
# define BUFFER_SIZE 32

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/socket.h>

int     ft_strlen_c(char *str, char c);
char    *ft_strjoin(char *s1, char *s);
char    *ft_strcut_c(char *post_buffer, char c);
char    *ft_strdup_c(char *str, char c);
int     is_a_c(char *str, char c);
int     get_next_line(int fd, char **line);

#endif