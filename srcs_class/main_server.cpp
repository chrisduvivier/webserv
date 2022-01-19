# include "ServerSocket.hpp"

# define PORT 8080

int main(int argc, char *argv[])
{
    // create a server socket instance and run it
    ServerSocket Server(PORT, INADDR_ANY, AF_INET, SOCK_STREAM, 0);
    Server.init();
    Server.run();
    
    return (0);
}