/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduvivie <cduvivie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 15:15:13 by cduvivie          #+#    #+#             */
/*   Updated: 2022/03/15 16:39:12 by cduvivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Cluster.hpp"

Cluster::Cluster()
{
    
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
