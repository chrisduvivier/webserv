#include "utils.hpp"

size_t	ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

/*
**	The strdup() function allocates sufficient memory for a copy of the
**	string s1, does the copy, and returns a pointer to it.
*/

char	*ft_strdup(const char *s1)
{
	char	*str;
	char	*p;

	if (!s1)
		return (NULL);
	if (!(str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1))))
		return (NULL);
	p = str;
	while (*s1)
		*p++ = *s1++;
	*p = '\0';
	return (str);
}

/*
**	- Description:
**  Allocates (with malloc(3)) and returns a new
**  string, result of the concatenation of s1 and s2
**	- Return value:
**	The new string. NULL if the allocation fails.
*/

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	char	*p;

	if (!s1 || !s2)
		return (NULL);
	if (!(res = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1))))
		return (NULL);
	p = res;
	while (*s1)
		*p++ = *s1++;
	while (*s2)
		*p++ = *s2++;
	*p = '\0';
	return (res);
}

/*
**	- Description
**	locates the FIRST occurrence of c (converted to a char)
**	in the string pointed to by s.  The terminating null character
**	is considered to be part of the string; therefore if c is `\0', the
**	functions locate the terminating `\0'.
**
**	- Return Values
**	return a pointer to the located
**	character, or NULL if the character does not appear in the string.
*/

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return (char *)(s);
		s++;
	}
	if (*s == (char)c)
		return (char *)(s);
	return (NULL);
}

/*
**	- Description:
**	Allocates (with malloc(3)) and returns a substring
**	from the string given in argument.
**	The substring begins at index ’start’ and is of maximum size ’len’.
**	- Return value:
**	The substring. NULL if the allocation fails.
*/

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;

	if (!(substr = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = 0;
	if (s && start < (unsigned int)ft_strlen(s))
	{
		while (s[start + i] && i < len)
		{
			substr[i] = s[start + i];
			i++;
		}
	}
	substr[i] = '\0';
	return (substr);
}


/*
**	- Description:
**	exit function which checks if freeing the stack[fd] is needed
**	before returning (-1).
*/

static int	free_and_exit(char **fd_str)
{
	if (fd_str && *fd_str)
	{
		free(*fd_str);
		*fd_str = NULL;
	}
	return (-1);
}

/*
**	- Description:
**	Append the `\n` or `\0` terminating string to line.
**	while doing so, store the excessive reading to the static memory.
**	ex:
**		"Hello, World!\n how are you?\0"
**		|    line    |     excess     |
**
**	=> stack[fd] = "how are you?\0"
**
**	If it reached EOF, simply duplicate the line and free the stack, return 0.
**	If malloc fails in any of the helper function, free(str[fd]) & return -1.
*/

static int	append_line(char **fd_str, char **line)
{
	char	*tmp;
	size_t	len;

	len = 0;
	while ((*fd_str)[len] != '\0' && (*fd_str)[len] != '\n')
		len++;
	if ((*fd_str)[len] == '\n')
	{
		*line = ft_substr(*fd_str, 0, len);
		tmp = ft_strdup(&((*fd_str)[len + 1]));
		free(*fd_str);
		*fd_str = tmp;
		if (!(*line) || !(*fd_str))
			return (free_and_exit(fd_str));
		return (1);
	}
	*line = ft_strdup(*fd_str);
	free(*fd_str);
	*fd_str = NULL;
	if (*line == NULL)
		return (free_and_exit(fd_str));
	return (0);
}

/*
**	- Description:
**	Handles the output value of gnl.
**	`-1` If the read cursor got a negative value (read failed).
**	If the cursor reached EOF and the stack is empty,
**	we return 0 with an empty line.
**	Otherwise, we call append_line and returns its returned value.
**
**	If malloc fails in any of the helper function, free(str[fd]) & return -1.
*/

static int	output(int cursor, char **stack, int fd, char **line)
{
	if (cursor == 0 && (!stack[fd]))
	{
		*line = ft_strdup("");
		if (!(*line))
			return (free_and_exit(&stack[fd]));
		return (0);
	}
	return (append_line(&stack[fd], line));
}

/*
**	- Description:
**	ft_read_gnl keeps what it reads on the stack (static char), and
**	extract the line once a `\n` or `\0` is found. If the stack[fd] exists,
**	which means the previous read call had more char after the `\n`, we join
**	them together.
**
**	- Return value:
**	cursor upon successful reading and memory allocation.
**	Otherwise, return (-1).
*/

static int	ft_read_gnl(char **stack, int fd)
{
	char	buf[BUFFER_SIZE + 1];
	char	*tmp;
	int		cursor;

	cursor = 0;
	while (!(ft_strchr(stack[fd], '\n'))
		&& (cursor = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[cursor] = '\0';
		if (!stack[fd])
		{
			stack[fd] = ft_strdup(buf);
			if (!stack[fd])
				return (-1);
		}
		else
		{
			tmp = ft_strjoin(stack[fd], buf);
			free(stack[fd]);
			stack[fd] = tmp;
			if (!stack[fd])
				return (-1);
		}
	}
	return (cursor);
}

/*
**	- Description:
**	Write a function which returns a line read from a fd, without the newline.
**	If a memory allocation fails at any point, we return -1.
**
**	- Return value:
**	1 : a line has been read.
**	0 : EOF has been reached.
**	-1 : An error happened.
**
**	- Error handling:
**	Once a malloc or read fails, we call free_and_exit to free the stack[fd].
**	Note that this will free only the stack[fd] which gave error.
*/

int			get_next_line(int fd, char **line)
{
	static char	*stack[OPEN_MAX];
	int			cursor;

	if (fd < 0 || fd >= OPEN_MAX || !line || BUFFER_SIZE < 1)
		return (-1);
	if ((cursor = ft_read_gnl(stack, fd)) < 0)
		return (free_and_exit(&stack[fd]));
	return (output(cursor, stack, fd, line));
}
