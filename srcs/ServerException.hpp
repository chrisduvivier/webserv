#ifndef SERVER_EXCEPTION_HPP
# define SERVER_EXCEPTION_HPP

# include <exception>
# include <string>

struct ServerException : public std::exception
{
    protected:
    	const char *errorMsg;

    public:
		ServerException (const char* str){
			errorMsg =  str;    
		}
		ServerException (const std::string str){
			errorMsg =  str.c_str();    
		}
		const char * what () const throw ()
		{
			return errorMsg;
		}
};

#endif
