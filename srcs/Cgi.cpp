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
	_GATEWAY_INTERFACE = "";
	_PATH_INFO = "";
	_PATH_TRANSLATED = "";
	_QUERY_STRING = "";
	_REMOTE_ADDR = "";
	_REMOTE_USER = "";
	_REQUEST_METHOD = "";
	_SCRIPT_NAME = "";
	_SERVER_NAME = "";
	_SERVER_PORT = "";
	_SERVER_SOFTWARE = "";
	// _REDIRECT_STATUS = "";
	_body = "";
	_env[0] = (char *)std::string("Hello this is my 1st env arg").c_str();
	_env[1] = (char *) std::string("Hello this is my 2nd env arg").c_str();
	_env[2] = NULL;

}

void    Cgi::set_var(HttpRequest request)
{
	char cwd[256];

	getcwd(cwd, 256);
	std::string full_path(cwd);
	// full_path += PUBLIC_HTML_FOLDER;

	_REQUEST_METHOD = request.get_method();
	_SERVER_PORT = "8080";
	_SERVER_SOFTWARE = "webserv";
	_GATEWAY_INTERFACE = "CGI/1.1";
	_SCRIPT_NAME = request.get_url();
	_PATH_INFO = request.get_url();
	_PATH_TRANSLATED = (full_path + request.get_url()).c_str();
	// std::string referer = request.get_arg_method();
	// if (referer.find('?') != std::string::npos)
	// {
	// 	_QUERY_STRING = referer.substr(referer.find('?') + 1);
	// }
	_SERVER_NAME = "http://localhost";  //TODO: get from server config
	// _REDIRECT_STATUS =  "200";
}

void	Cgi::set_env()
{
	_env[0] = (char*)"SERVER_PROTOCOL=HTTP/1.1";
	_env[1] = strcat("REQUEST_METHOD=", _REQUEST_METHOD);
	_env[2] = strcat("PATH_INFO=", _PATH_INFO);
	_env[3] = strcat("SERVER_PORT=", _SERVER_PORT);
	_env[4] = strcat("SERVER_SOFTWARE=", _SERVER_SOFTWARE);
	_env[5] = strcat("GATEWAY_INTERFACE=", _GATEWAY_INTERFACE);
	_env[6] = strcat("SCRIPT_FILENAME=", _SCRIPT_NAME);
	_env[7] = strcat("PATH_TRANSLATED=", _PATH_TRANSLATED);
	_env[8] = strcat("QUERY_STRING=", _QUERY_STRING);
	_env[9] = strcat("REMOTE_ADDR=", _REMOTE_ADDR);
	_env[10] =strcat("REMOTE_USER=", _REMOTE_USER);
	_env[11] =strcat("SERVER_NAME=", _SERVER_NAME);
	_env[12] =strcat("REDIRECT_STATUS=", "200");
	_env[13] = NULL;
}


// execute the cgi file using fork
int     Cgi::execute_cgi(std::string path_to_cgi)
{ 
	std::cout << "== execute_cgi ==\n";
	int fd_pipe[2];
	int fdin = dup(STDIN_FILENO);
	if (fdin < 0)
		std::cout << "Error: dup\n";
	int fdout = dup(STDOUT_FILENO);
	if (fdin < 0)
		std::cout << "Error: dup\n";

	char buffer[BUFFER_SIZE + 1];

	if (pipe(fd_pipe) < 0)
		std::cout << "Error: pipe\n";
		
	int cgi_pid = fork();
	if (cgi_pid == 0) // child process)
	{
		// dup2(old, new) function copies the old_file_descriptor into the new_file_descriptor.
		if (dup2(fd_pipe[0], STDIN_FILENO) < 0)    // [    0     |     1     ]
			std::cout << "Error: dup2\n";
		if (dup2(fd_pipe[1], STDOUT_FILENO) < 0)   // [ read end | write end ]
			std::cout << "Error: dup2\n";

		close(fd_pipe[0]);	//close read end side
		std::cout << "== execve: ["<< path_to_cgi << "]\n";
		if (execve(path_to_cgi.c_str(), NULL, _env) < 0)
		{
			std::cout << "Error: cgi scipt execution error\n";
			return (-1);      //TODO, just check error of execve
		}
		exit(0);
	}
	close(fd_pipe[1]);	//close write side
	
	std::cout << "	HEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEERE	\n";

	if (dup2(fd_pipe[0], STDIN_FILENO) < 0)	// STDIN -> READING SIDE OF PIPE
		std::cout << "Error: dup2\n";

	int ret = 1;
	std::string returned_body = "";
	std::cout << "== execute_cgi: copy to returned_body ==\n";
	while (ret > 0) // retrieve output of cgi script
	{
		memset(buffer, 0, BUFFER_SIZE);
		ret = read(fd_pipe[0], buffer, BUFFER_SIZE);
		returned_body += buffer;
		std::cout << "== CGI: [" << returned_body << "]\n";
	}
	close(fd_pipe[0]);

	_body = returned_body;
	if (dup2(fdin, STDIN_FILENO) < 0)
		std::cout << "Error: dup00\n";
	if (dup2(fdout, STDOUT_FILENO) < 0)
		std::cout << "Error: dup11\n";

	return (ret);
};

std::string Cgi::get_body() const { return (_body); };