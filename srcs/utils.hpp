#ifndef UTILS_HPP
# define UTILS_HPP

# include <string.h>
# include <string>
# include <iostream>
# include <vector>
# include <stdlib.h>
#include <sstream>

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define RESET "\033[0m"

char*	strcat(std::string s1, std::string s2);

class nullptr_t
{
	public:
		template<class T>
		inline operator T*() const // convertible to any type of null non-member pointer...
		{ return 0; }

		template<class C, class T>
		inline operator T C::*() const   // or any type of null member pointer...
		{ return 0; }

	private:
		void operator&() const;  // Can't take address of nullptr
};

void			error_exit(std::string	error_msg); // used inside ServerConfig and ConfigFile
std::string		ip_to_string(int *host); // used to convert an ip array to a string

#endif
