# include "ServerSocket.hpp"
#include "ServerConfig.hpp"

# define PORT 8080

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

	ServerConfig		server_info[20]; // maxed the numb of servers to 20 for now

	//	Getters and their use
	/*std::cout << server_info[0].get_port()[0] << std::endl;
	std::cout << server_info[0].get_server_names()[0] << std::endl;
	std::cout << server_info[0].get_client_max_body_size() << std::endl;
	std::map<int, std::string>::iterator	it1;
	std::map<int, std::string>				error_page = server_info[0].get_error_pages();
	it1 = error_page.begin();
	std::cout << "error_nb = "<< it1->first << " and path = " << it1->second << std::endl;*/

    // create a server socket instance and run it
    ServerSocket Server(PORT, INADDR_ANY, AF_INET, SOCK_STREAM, 0);
    try {
        Server.init();
    } catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return (-1);
	}

    try {
        Server.run();
    } catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return (-1);
	}

    return (0);
}
