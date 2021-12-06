#include "mini_serv.hpp"

int listen_fd;

void        signal_handler(int signum) {
    std::cout << "(" << signum << ") Serveur ending..." << std::endl;
    close(listen_fd);
    exit(1);
}

void        mini_serv(int port) {

    int                 client_fd;
    struct sockaddr_in  listen_addr;


    // ctrl+c handling to close socket
    signal(SIGINT, signal_handler);

    // create socket
    if (listen_fd = socket(AF_INET, SOCK_STREAM, 0) < -1)
    {
        std::cerr << "error: socket creation" << std::endl;
        exit(1);
    }

    // set address type to accept
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    listen_addr.sin_port = htons(port);

    // link address to socket
    if ( bind(listen_fd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) == -1 )
    {
        std::cerr << "error: socket creation" << std::endl;
        exit(1);
    }

    // turn socket in listen mode
    /* 
       (5=) The backlog argument defines the maximum length to which the
       queue of pending connections for sockfd may grow
    */
    if ( (listen(listen_fd, 5)) == -1)
    {
        std::cerr << "error: listen call" << std::endl;
        exit(1);
    }

    // --setup done-- //

    std::list<char *> request;
    while (true)
    {
        if ( (client_fd = accept(listen_fd, NULL. NULL)) == -1) 
        {
            std::cerr << "error: accept call" << std::endl;
            exit(1);
        }
        
        // announce_client(client_fd);

        // request = request_reader(client_fd);
        // print_request(request);
        // request.clear();

		// response(client_fd);

        // close(client_fd);
    }
}
