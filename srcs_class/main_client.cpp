# include "ClientSocket.hpp"

# define PORT 8080

int main(int argc, char *argv[])
{
    // create a server socket instance and run it
    ClientSocket Client(PORT, INADDR_ANY, AF_INET, SOCK_STREAM, 0);
    Client.init();
    Client.run();
    
    return (0);
}