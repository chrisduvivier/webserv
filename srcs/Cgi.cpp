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
	_CONTENT_LENGTH = "";
	_GATEWAY_INTERFACE = "CGI/1.1";
	_PATH_INFO = "";
	_PATH_TRANSLATED = "";
	_QUERY_STRING = "";
	_REMOTE_ADDR = "";
	_REMOTE_USER = "";
	_REQUEST_METHOD = "";
	_SCRIPT_FILENAME = "";
	_SCRIPT_NAME = "";
	_SERVER_NAME = "";
	_SERVER_PROTOCOL = "HTTP/1.1";
	_SERVER_PORT = "";
	_SERVER_SOFTWARE = "";
	// _REDIRECT_STATUS = "";
	_body = "";
	_error = false;
	_env[0] = NULL;
}

void    Cgi::set_var(HttpRequest request)
{
	char cwd[256];

	getcwd(cwd, 256);
	std::string full_path(cwd);
	// full_path += PUBLIC_HTML_FOLDER;

	_CONTENT_LENGTH = request.get_headers().at("Content-Length");
	_REQUEST_METHOD = request.get_method();
	_PATH_INFO = request.get_url();
	_SERVER_PORT = "8080";
	_SERVER_SOFTWARE = "webserv";
	_SCRIPT_FILENAME = (full_path + request.get_url()).c_str();
	_SCRIPT_NAME = request.get_url();
	_PATH_TRANSLATED = (full_path + request.get_url()).c_str();
	_QUERY_STRING = request.get_body();
	// std::string referer = request.get_arg_method();
	// if (referer.find('?') != std::string::npos)
	// {
		// _QUERY_STRING = referer.substr(referer.find('?') + 1);
	// }
	_SERVER_NAME = "http://localhost";  //TODO: get from server config
	// _REDIRECT_STATUS =  "200";
}

void	Cgi::set_env()
{
	_env[0] = strcat("CONTENT_LENGTH=", 	_CONTENT_LENGTH);
	_env[1] = strcat("GATEWAY_INTERFACE=", 	_GATEWAY_INTERFACE);
	_env[2] = strcat("PATH_INFO=", 			_PATH_INFO);
	_env[3] = strcat("PATH_TRANSLATED=", 	_PATH_TRANSLATED);
	_env[4] = strcat("QUERY_STRING=", 		_QUERY_STRING);
	_env[5] = strcat("REMOTE_ADDR=", 		_REMOTE_ADDR);
	_env[6] = strcat("REMOTE_USER=", 		_REMOTE_USER);
	_env[7] = strcat("REQUEST_METHOD=", 	_REQUEST_METHOD);
	_env[8] = strcat("SCRIPT_FILENAME=", 	_SCRIPT_FILENAME);
	_env[9] = strcat("SCRIPT_NAME=", 		_SCRIPT_NAME);
	_env[10] = strcat("SERVER_NAME=", 		_SERVER_NAME);
	_env[11] = strcat("SERVER_PROTOCOL=", 	_SERVER_PROTOCOL);
	_env[12] = strcat("SERVER_PORT=", 		_SERVER_PORT);
	_env[13] = strcat("SERVER_SOFTWARE=", 	_SERVER_SOFTWARE);
	_env[14] = NULL;
	print_env();
}

void	Cgi::print_env()
{
	for (int i = 0; i < 13; i++)
	{
		std::cerr << "ENV: " << _env[i] << std::endl;
	}
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
	this->set_env();

	int cgi_pid = fork();
	if (cgi_pid == 0) // child process)
	{
		// dup2(old, new) function copies the old_file_descriptor into the new_file_descriptor.
		dup2(fd_pipe[0], STDIN_FILENO);    // [    0     |     1     ]
		dup2(fd_pipe[1], STDOUT_FILENO);   // [ read end | write end ]
		if (execve(path_to_cgi.c_str(), NULL, _env) < 0)
		{
			std::cout << RED << "Error: CGI EXEC\n" << RESET;
			return (-1);
		}
		exit(0);
	}
	close(fd_pipe[1]);	//close write side
	
	waitpid(-1, NULL, 0);

	int ret = 1;
	while (ret > 0)
	{
		memset(buffer, 0, BUFFER_SIZE);
		ret = read(fd_pipe[0], buffer, BUFFER_SIZE);
		_body += buffer;
		
	}
	if (dup2(fdin, STDIN_FILENO) < 0)
		std::cout << "Error: dup00\n";
	if (dup2(fdout, STDOUT_FILENO) < 0)
		std::cout << "Error: dup11\n";
	return (ret);
};

std::string Cgi::get_body() const { return (_body); };