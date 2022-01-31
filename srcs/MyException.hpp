#ifndef MY_EXCEPTION_HPP
# define MY_EXCEPTION_HPP

# include <exception>
# include <string>

struct MyException : public std::exception
{
    protected:
    	const char *errorMsg;

    public:
		MyException (const char* str){
			errorMsg =  str;    
		}
		MyException (const std::string str){
			errorMsg =  str.c_str();    
		}
		const char * what () const throw ()
		{
			return errorMsg;
		}
};

#endif
