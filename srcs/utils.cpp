#include "utils.hpp"

char*	strcat(std::string s1, std::string s2)
{
	return (strdup( (char*)(s1 + s2).c_str() ));
}


