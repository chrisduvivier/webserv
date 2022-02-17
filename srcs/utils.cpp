#include "utils.hpp"

char*	strcat(std::string s1, std::string s2)
{
	return (strdup( (char*)(s1 + s2).c_str() ));
}

int		check_first_word(std::string word, std::string	block)
{
	int i = block.find(word);
	int y = i;
	i--;
	if (i != (int)std::string::npos)
	{
		while (i > 0 && block[i] != '\n')
		{
			std::cout << block[i] << std::endl;
			if (!isspace(block[i]))
				return ((int)std::string::npos);
			i--;
		}
	}
	else	
		return ((int)std::string::npos);
	return (y);
}

std::vector<std::string>		set_vector(std::string word, std::string block)
{
	std::vector<std::string>	vec;
	int i = check_first_word(word, block);
	int y = i;
	if (i == -1)
	{
		vec.push_back("GET");
		vec.push_back("POST");
		vec.push_back("DELETE");
		return (vec);
	}
	else
	{
		while (i < (int)block.size() || block[i] == '\n')
			i++;
		std::string	temp = block.substr(y, i);
		if ((int)temp.find("GET") != -1)
			vec.push_back("GET");
		if ((int)temp.find("POST") != -1)
			vec.push_back("POST");
		if ((int)temp.find("DELETE") != -1)
			vec.push_back("DELETE");
	}
	return (vec);
}

std::string		set_string(std::string word, std::string block)
{
	int i = check_first_word(word, block);
	std::string	temp;

	if (i == -1)
		return (temp);
	else
	{
		while (isalnum(block[i]) && i < (int)block.size() && block[i] != '\n')
			i++;
		while (i < (int)block.size() && isspace(block[i]))
			i++;
		while (i < (int)block.size() && block[i] != '\n')
		{
			temp.push_back(block[i]);
			i++;
		}
	}
	return (temp);
}

int			set_int(std::string word, std::string block)
{
	int i = check_first_word(word, block);
	std::string temp;

	if (i == -1)
		return (0);
	else
	{
		
		while (isalnum(block[i]) && i < (int)block.size() && block[i] != '\n')
			i++;
		while (i < (int)block.size() && isspace(block[i]))
			i++;
		while (i < (int)block.size() && block[i] != '\n')
		{
			temp.push_back(block[i]);
			i++;
		}
	}
	return (atoi(temp.c_str()));
}