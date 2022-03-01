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
