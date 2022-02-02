# include "ClientSocket.hpp"

# define PORT 14000

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    // create a server socket instance and run it
    ClientSocket Client(PORT, INADDR_ANY, AF_INET, SOCK_STREAM, 0);
    Client.init();
    Client.run();
    
    return (0);
}