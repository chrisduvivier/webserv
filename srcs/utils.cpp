#include "utils.hpp"

char*	strcat(std::string s1, std::string s2)
{
	return (strdup( (char*)(s1 + s2).c_str() ));
}

void	error_exit(std::string	error_msg)
{
	std::cerr << error_msg;
	exit(1);
}

std::string	ip_to_string(int *host)
{
	int	i = 0;
	std::stringstream ss;

	while (i < 4)
	{
		ss << (host[i]);
		if (i != 3)
			ss << ".";
		i++;
	}
	return (ss.str());
}