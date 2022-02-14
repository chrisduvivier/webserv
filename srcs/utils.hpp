#ifndef UTILS_HPP
# define UTILS_HPP

# include <string.h>
# include <string>

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

#endif