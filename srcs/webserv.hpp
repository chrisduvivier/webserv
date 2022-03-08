#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# define PUBLIC_HTML_FOLDER "public_html"

/* Macro to set the appropriate Pyhton3 interpreter depending on the OS */
# ifndef PYTHON_INTERPRETER
#  ifdef __linux__  //Linux
#   define PYTHON_INTERPRETER "/usr/bin/python3"
#  endif
#  ifdef __APPLE__  //MAC OS
#   define PYTHON_INTERPRETER "/usr/local/bin/python3"
#  endif
# endif

#endif
