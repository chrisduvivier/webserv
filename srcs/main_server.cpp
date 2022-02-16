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
	std::map<int, std::string>::iterator	it3;
	std::map<int, std::string>				error_page = it1->get_error_pages();
	it2 = error_page.begin();
	it3 = error_page.end();
	while (it2 != it3)
	{
		std::cout << "error_nb = "<< it2->first << " and path = " << it2->second << std::endl;
		it2++;
	}
	std::map<std::string, Location>::iterator	it4;
	std::map<std::string, Location>::iterator	it5;
	std::map<std::string, Location>				location = it1->get_location();
	it4 = location.begin();
	it5 = location.end();
	std::vector<std::string>					method = it4->second.get_method();
	while (it4 != it5)
	{
		std::cout << "location = "<< it4->first << " ==> redirection = " << it4->second.get_redirection() << std::endl;
		std::cout << "location = "<< it4->first << " ==> listing = " << it4->second.get_listing() << std::endl;
		std::cout << "location = "<< it4->first << " ==> directory = " << it4->second.get_directory() << std::endl;
		it4++;
	}
	std::vector<std::string>::iterator		it_method = method.begin();
	std::vector<std::string>::iterator		it2_method = method.end();
	while (it_method != it2_method)
	{
		std::cout << "Method = " << *it_method << std::endl;
		it_method++;
	}*/
	
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
