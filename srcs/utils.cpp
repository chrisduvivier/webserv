#include "utils.hpp"

char*	strcat(std::string s1, std::string s2)
{
	return (strdup( (char*)(s1 + s2).c_str() ));
}

void	error_exit(std::string	error_msg)
{
	std::cerr << error_msg;
	exit(1);
}

std::string	ip_to_string(int *host)
{
	int	i = 0;
	std::stringstream ss;

	while (i < 4)
	{
		ss << (host[i]);
		if (i != 3)
			ss << ".";
		i++;
	}
	return (ss.str());
}

std::map<std::string, std::string> ContentTypeList(){

	std::map<std::string, std::string> contentType;
	contentType["html"] = "text/html; charset=utf-8";
	contentType["txt"] = "text/plain; charset=utf-8";
	contentType["jpeg"] = "image/jpeg";
	contentType["png"] = "image/png";
	contentType["gif"] = "image/gif";
	contentType["bmp"] = "image/bmp";
	contentType["webp"] = "image/webp";
	contentType["ico"] = "image/vnd.microsoft.icon";
	contentType["cgi"] = "cgi";
	return (contentType);
}

int	is_directory(const char *path) {

	struct stat stats;

    stat(path, &stats);
    if (S_ISDIR(stats.st_mode))
        return 1;
    return 0;
}

std::string	get_file_ext(std::string path) {

	std::string extension;
	size_t pos = path.find('.', 1);
	if (pos != std::string::npos)
	{
		extension = path.substr(pos + 1);
		pos = extension.find("?");
		if (pos != std::string::npos)
			extension = extension.erase(pos, std::string::npos);
	}
	return (extension);
}

std::string	itostr(int nbr) { 

	std::stringstream ss;

	ss << nbr;
	return (ss.str());
}