/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduvivie <cduvivie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 15:15:11 by cduvivie          #+#    #+#             */
/*   Updated: 2022/03/17 14:36:50 by cduvivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include <unistd.h>

# include <string>
# include <iostream>
# include <vector>
# include <map>

/*
**	Cluster holds a queue of HttpResponse (as string) to be returned to the client.
**	The queue is accessed via the map, where the key is the client_socket (int), and value is the vector of responses.
*/

class Cluster
{
	public:
		
		Cluster();
		
		Cluster (const Cluster &copy)
		{
			this->_response_queue = copy._response_queue;
		}
		Cluster & operator = (const Cluster &rhs)
		{
			if (this != &rhs)
				this->_response_queue = rhs._response_queue;
			return *this;
		}

		bool	find(int client_sock);
		void    insert(int client_sock, std::string response);
		void	print();
		
		std::map<int, std::vector<std::string> >    get_response_queue();
		std::vector<std::string>					get_queue(int client_sock);

	private:
		std::map<int, std::vector<std::string> >	_response_queue;

};

#endif
