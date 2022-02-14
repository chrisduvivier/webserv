#include "ServerSocket.hpp"
#include "ServerConfig.hpp"
#include "ConfigFile.hpp"
#include <vector>

# define PORT 8080

int main(int argc, char *argv[])
{
	int i = 0;
    if (argc > 2)
	{
		std::cerr << "Invalid args : use a config file as arg\n";
		return (1);
	}
	ConfigFile conf_file((char *)argv[1]);
	std::vector<ServerConfig>	Server_vector;

	while (i < conf_file.get_server_nb())
	{
		Server_vector.push_back(conf_file.populate(i));
		/*std::cout << "server_vector.pushback created \n";*/
		i++;
	}

	//	Getters and their use
	/*std::vector<ServerConfig>::iterator it1 = Server_vector.begin();
	std::cout << it1->get_port() << std::endl;
	std::cout << it1->get_server_names()[0] << std::endl;
	std::cout << it1->get_client_max_body_size() << std::endl;
	std::cout << it1->get_host_name() << std::endl;
	std::map<int, std::string>::iterator	it2;
	std::map<int, std::string>				error_page = it1->get_error_pages();
	it2 = error_page.begin();
	std::cout << "error_nb = "<< it2->first << " and path = " << it2->second << std::endl;
	std::map<std::string, std::string>::iterator	it3;
	std::map<std::string, std::string>				location = it1->get_location();
	it3 = location.begin();
	std::cout << "location = "<< it3->first << "\nblock = \n" << it3->second << std::endl;*/

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
