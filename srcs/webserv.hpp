#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# define PUBLIC_HTML_FOLDER "public_html"

/* Macro to set the appropriate Pyhton3 interpreter depending on the OS */
# ifndef PYTHON_INTERPRETER
#  ifndef __APPLE__  //not MAC OS -> Linux
#   define PYTHON_INTERPRETER "/usr/bin/python3"
#  endif
#  ifndef __linux__  //not Linux -> MAC OS
#   define PYTHON_INTERPRETER "/usr/local/bin/python3"
#  endif
# define PYTHON_INTERPRETER "/usr/local/bin/python3"    //default to MACOS
# endif

#endif
