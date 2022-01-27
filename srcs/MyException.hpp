#ifndef MY_EXCEPTION_HPP
# define MY_EXCEPTION_HPP

# include <exception>

struct MyException : public std::exception
{
    protected:
    	const char *errorMsg;

    public:
		MyException (const char* str){
			errorMsg =  str;    
		}
		const char * what () const throw ()
		{
			return errorMsg;
		}
};

#endif
