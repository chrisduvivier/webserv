#ifndef UTILS_HPP
# define UTILS_HPP

# include <string.h>
# include <string>
# include <iostream>
# include <vector>
# include <stdlib.h>

# include <map>
//used to identify directory
# include <sys/stat.h>
//used to read into a directory
# include <sys/types.h>
# include <dirent.h>

#include <sstream>

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define RESET "\033[0m"

# define DEBUG(MSG) std::cout << "\033[0;35m\e[1m" << MSG << "\e[0m\033[0m" << std::endl;

char*	strcat(std::string s1, std::string s2);

/* 
* "converting ... to string" macro
* https://stackoverflow.com/a/5590404
*/
# include <sstream>
# define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

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

/*
	--
		This function returns a map that contains ressources extension that are supported on the server as key
		and their corresponding Content-Type as value
	--
*/
std::map<std::string, std::string> ContentTypeList();

/*
	--
		This function returns 1 if an url leads to a directory, 0 if it does not
	--
*/
int	is_directory(const char *path);

/*
	--
		This function cuts the extension of an url and returns it
	--
*/
std::string	get_file_ext(std::string path);

/*
	--
		This function takes an integer and returns its std::string representation
	--
*/
std::string	itostr(int nbr);

#endif
