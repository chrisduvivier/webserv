/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduvivie <cduvivie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 15:15:11 by cduvivie          #+#    #+#             */
/*   Updated: 2022/03/15 15:15:31 by cduvivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLUSTER_HPP
# define CLUSTER_HPP

#include <unistd.h>

# include <string>
# include <iostream>
# include <vector>
# include <map>

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
		std::map<int, std::vector<std::string> >	_response_queue;
		void	print();

	private:

};

#endif
