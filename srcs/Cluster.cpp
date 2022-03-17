/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduvivie <cduvivie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 15:15:13 by cduvivie          #+#    #+#             */
/*   Updated: 2022/03/17 15:39:34 by cduvivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Cluster.hpp"

Cluster::Cluster()
{
    
}

bool     Cluster::find(int client_sock)
{
    if (this->_response_queue.find(client_sock) == this->_response_queue.end())
        return (false);
    return (true);
}

void    Cluster::insert(int client_sock, std::string response)
{
    if (this->find(client_sock) == false) // vector of response associated to that socket doesn't exists
	{
		// create vector with 1 string inside for initilaization
		std::vector<std::string> tmp;
		tmp.push_back(response);
		this->_response_queue.insert(std::pair<int, std::vector<std::string> >(client_sock, tmp));
	}
	else
    {
        if (this->get_queue(client_sock).size() > 10)
        {
            //delete the oldest element (in our implementation, this is most likely a dead request object)
            this->get_queue(client_sock).erase(this->get_queue(client_sock).begin());
        }    
		this->get_queue(client_sock).push_back(response);
    }
}

void    Cluster::print()
{
	std::map<int, std::vector<std::string> >::iterator it = _response_queue.begin();

    // Iterate over the map using Iterator till end.
    while (it != _response_queue.end())
    {
        // Accessing KEY from element pointed by it.
        int socket_number = it->first;
        // Accessing VALUE from element pointed by it.
        std::cout << "socket_number=[" << socket_number << "]\n"; 
        for (std::vector<std::string>::iterator it_vec = it->second.begin();  it_vec != it->second.end(); it_vec++)
        {
            std::cout << "string=[" << *it_vec << "]" << std::endl;
        }
        // Increment the Iterator to point to next entry
        it++;
    }
}

std::map<int, std::vector<std::string> >    Cluster::get_response_queue()
{
    return (this->_response_queue);
}

std::vector<std::string>					Cluster::get_queue(int client_sock)
{
    return (this->_response_queue.find(client_sock)->second);
}
