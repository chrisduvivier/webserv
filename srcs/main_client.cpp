# include "ClientSocket.hpp"

# define PORT 8080

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    std::string ip = "127.0.0.1";
    // create a server socket instance and run it
    ClientSocket Client(PORT, ip, AF_INET, SOCK_STREAM, 0);
    Client.init();
    Client.run();
    
    return (0);
}