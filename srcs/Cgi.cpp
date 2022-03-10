#include "Cgi.hpp"

Cgi::Cgi(){
	init_var();
}


Cgi::Cgi(HttpRequest request, std::string path_to_cgi, ServerConfig serv)
{
	init_var();
	set_var(request, path_to_cgi, serv);
}

Cgi::~Cgi(){
	free_env();
}

void    Cgi::init_var()
{
	_CONTENT_LENGTH = 		"";
	_GATEWAY_INTERFACE = 	"CGI/1.1";
	_PATH_INFO = 			"";
	_PATH_TRANSLATED = 		"";
	_QUERY_STRING = 		"";
	_REMOTE_ADDR = 			"";
	_REMOTE_USER = 			"";
	_REQUEST_METHOD = 		"";
	_SCRIPT_FILENAME = 		"";
	_SCRIPT_NAME = 			"";
	_SERVER_NAME = 			"";
	_SERVER_PROTOCOL = 		"HTTP/1.1";
	_SERVER_PORT = 			"";
	_SERVER_SOFTWARE = 		"";
	_body = 				"";
	_error = 				false;
	_env[0] = 				NULL;
	_args[0] = 				NULL;
}

void    Cgi::set_var(HttpRequest request, std::string path_to_cgi, ServerConfig serv)
{
	char cwd[256];

	getcwd(cwd, 256);
	std::string full_path(cwd);

	if (request.get_headers().count("Content-Length"))
		_CONTENT_LENGTH = request.get_headers().at("Content-Length");
	_REQUEST_METHOD = request.get_method();
	_PATH_INFO = request.get_url();
	_SERVER_PORT = SSTR(serv.get_port());
	_SERVER_SOFTWARE = "webserv";
	_SCRIPT_FILENAME = (full_path + request.get_url()).c_str();
	_SCRIPT_NAME = request.get_url();
	_PATH_TRANSLATED = (full_path + request.get_url()).c_str();
	if (request.get_method() == "GET")
	{
		_QUERY_STRING = request.get_query_string();
		_CONTENT_LENGTH = _QUERY_STRING.length();
	}
	_SERVER_NAME = "http://localhost";  //TODO: get from server config

	/* set args for the execve */
	_args[0] = strdup((char *)PYTHON_INTERPRETER);		//full path of Python3 interpreter
	_args[1] = strdup((char *)path_to_cgi.c_str());
	_args[2] = NULL;
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

void	Cgi::free_env()
{
	for (int i = 0; i < 14; i++)
	{
		if (_env[i])
		{
			free(_env[i]);
			_env[i] = NULL;
		}
	}
	if (_args[0])
		free(_args[0]);
	if (_args[1])
		free(_args[1]);
}

void	Cgi::print_env()
{
	for (int i = 0; i < 14; i++)
	{
		std::cerr << "ENV: " << _env[i] << std::endl;
	}
}

// execute the cgi file using fork
int     Cgi::execute_cgi(HttpRequest request)
{ 
	std::cout << "== execute_cgi ==\n";
	
	int fd_pipe[2];
	int fdin = dup(STDIN_FILENO);
	if (fdin < 0)
	{
		std::cerr << "Error: dup\n";
		return (-1);
	}

	char buffer[BUFFER_SIZE + 1];
	if (pipe(fd_pipe) < 0)
	{
		std::cerr << "Error: pipe\n";
		return (-1);
	}
	this->set_env();

	int cgi_pid = fork();
	if (cgi_pid == 0) // child process)
	{
		dup2(fd_pipe[0], STDIN_FILENO);    // [    0     |     1     ]
		dup2(fd_pipe[1], STDOUT_FILENO);   // [ read end | write end ]
		close(fdin);
		close(fd_pipe[0]);
		close(fd_pipe[1]);
		if (execve(PYTHON_INTERPRETER, _args, _env) < 0)
		{
			std::cerr << RED << "Error: CGI EXEC\n" << RESET;
			return (-1);
		}
		exit(0);
	}
	
	// write the body of the request to the stdIN of cgi
	if (request.get_method() == "POST")
	{
		std::cout << "Parent process write to cgi: string=[" << request.get_body().c_str() << "] with length=[" << request.get_body().length() << "]\n";
		int res = write(fd_pipe[1], request.get_body().c_str(), request.get_body().length());
		std::cout << "result of write = [" << res << "]\n";

	}
	close(fd_pipe[1]);	//close write side

	int cgi_status = 0;
	waitpid(cgi_pid, &cgi_status, 0);
	if (WEXITSTATUS(cgi_status))
		return (-1);

	int ret = 1;
	while (ret > 0)
	{
		memset(buffer, 0, BUFFER_SIZE);
		ret = read(fd_pipe[0], buffer, BUFFER_SIZE);
		if (ret < 0)
			return (-1);
		_body += buffer;	
	}
	if (dup2(fdin, STDIN_FILENO) < 0)
		std::cerr << "Error: dup00\n";
	close(fd_pipe[0]);
	close(fdin);
	return (ret);
};

std::string Cgi::get_body() const { return (_body); };