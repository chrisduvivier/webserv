#include "ServerSocket.hpp"
#include "ServerConfig.hpp"
#include "ConfigFile.hpp"
#include <vector>
#include <string>

# define PORT 8080

# define DEFAULT_CONF "./conf/default.conf"

int main(int argc, char *argv[])
{
	int i = 0;
    std::string path = DEFAULT_CONF;
	if (argc == 2)
		path = argv[1];
	else if (argc > 2)
	{
		std::cout << "Invalid number of arguments : webserv [conf.file]\n";
		return (1);
	}
	ConfigFile conf_file((char *)path.c_str());
	std::vector<ServerConfig>	Server_vector;

	while (i < conf_file.get_server_nb())
	{
		Server_vector.push_back(conf_file.populate(i));
		i++;
	}

	// create a server socket instance and run it
	ServerSocket Server(Server_vector, AF_INET, SOCK_STREAM, 0);

	try {
		Server.run();
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return (-1);
	}
    return (0);
}
