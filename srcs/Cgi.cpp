#include "Cgi.hpp"

Cgi::Cgi(){
    init_var();
}

Cgi::Cgi(HttpRequest request)
{
    init_var();
    set_var(request);
}

void    Cgi::init_var()
{
    _PATH_INFO = "";
    _REQUEST_METHOD = "";
    _SERVER_PORT = "";
    _SERVER_SOFTWARE = "";
    _GATEWAY_INTERFACE = "";
    _SCRIPT_NAME = "";
    _PATH_TRANSLATED = "";
    _QUERY_STRING = "";
    _REMOTE_ADDR = "";
    _REMOTE_USER = "";
    _SERVER_NAME = "";
    // _REDIRECT_STATUS = "";
    _body = "";
    _env[0] = NULL;
}

void    Cgi::set_var(HttpRequest request)
{
    char cwd[256];

	getcwd(cwd, 256);
	std::string full_path(cwd);
	// full_path += PUBLIC_HTML_FOLDER;

	// _REQUEST_METHOD = request.get_method();
	// _PATH_INFO = request.get_arg_method();
	// _SERVER_PORT = parser.get_port()[0];
	// _SERVER_SOFTWARE = parser.get_server_name();
	// _GATEWAY_INTERFACE = "CGI/1.1";
	_SCRIPT_NAME = request.get_url();
	// _PATH_TRANSLATED = (char*)full_path.c_str();
	// std::string referer = request.get_arg_method();
	// if (referer.find('?') != std::string::npos)
	// {
	// 	_QUERY_STRING = referer.substr(referer.find('?') + 1);
	// 	_PATH_INFO = referer.substr(0, referer.find('?') );
	// 	_PATH_TRANSLATED += PATH_INFO;
	// }
	// _SERVER_NAME = "http://localhost";
	// _REDIRECT_STATUS =  "200";
}

// execute the cgi file using fork
int     Cgi::execute_cgi()
{ 
    int fd_pipe[2];
    int fdin = dup(STDIN_FILENO);
    int fdout = dup(STDOUT_FILENO);
    char buffer[BUFFER_SIZE + 1];

    pipe(fd_pipe);	
    if (!fork())
    {
        dup2(fd_pipe[0], STDOUT_FILENO);    // [    0     |     1     ]
        dup2(fd_pipe[1], STDIN_FILENO);     // [ read end | write end ]
        close(fd_pipe[0]);
        // execve(_SCRIPT_NAME.c_str(), NULL, _env);
        execve("cgi-bin/env.cgi", NULL, _env);
        exit(0);
    }
    close(fd_pipe[1]);
    dup2(fd_pipe[0], 1);

    int ret = 1;
    std::string returned_body = "";

    while (ret > 0) // retrieve output of cgi script
    {
        memset(buffer, 0, BUFFER_SIZE);
        ret = read(fd_pipe[0], buffer, BUFFER_SIZE);
        returned_body += buffer;
    }
    _body = returned_body;
    dup2(fdin, STDIN_FILENO);
    dup2(fdout, STDOUT_FILENO);
    return (ret);
};

std::string Cgi::get_body() const { return (_body); };