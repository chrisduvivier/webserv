#include "server.hpp"

void    print_usage(void) {
    std::cout << "webserv USAGE :" << std::endl;
    std::cout << "./webserv [PORT]" << std::endl;
    std::cout << "    [PORT] : number between 0-65535 is the port to listen. (don't use common port)" << std::endl;
}

int	parse_input(int argc, char *argv[])
{
	(void) argv;
	if (argc != 2) {
        print_usage();
        return (1);
    };
	return (0);
}

/* try instanciate server as a class */
int		main(int argc, char **argv)
{
    int     running = 1;  /* 1 if server should be running; 0 otherwise */
    Server	server;
	
	/* Verify if input is valid */
	if (parse_input(argc, argv))
		exit(0);
	
	/* initialize server */
	server.initialize(atoi(argv[1]));

    while (running)
    {
		if (server.performSelect() < 0)
			break ;
    } // end while running
        
 
    /*		close all open connections gracefully */
    /*		plus whatever other cleanup is needed.*/
    // clean_up();
	return (0);
}