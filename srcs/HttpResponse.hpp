/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduvivie <cduvivie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 15:15:40 by cduvivie          #+#    #+#             */
/*   Updated: 2022/03/15 15:15:41 by cduvivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <fstream>
# include <map>
# include <vector>

# include "utils.hpp"

# include "HttpRequest.hpp"
# include "ServerException.hpp"

# include "Cgi.hpp"

# include "ConfigFile.hpp"
# include "ServerConfig.hpp"


class HttpResponse
{
	public:

		HttpResponse();
		HttpResponse(HttpRequest request , ServerConfig serv);
		// ~HttpResponse();

		HttpResponse (const HttpResponse &copy) 
		{
			this->_protocol = copy._protocol;
			this->_status_code = copy._status_code;
			this->_status_text = copy._status_text;
			this->_headers = copy._headers;
			this->_body = copy._body;
			this->_response = copy._response;
			this->_serv = copy._serv;
			this->_req = copy._req;
		}

		HttpResponse & operator = (const HttpResponse &rhs)
		{
			// TODO: deep copy this
			if (this != &rhs)
			{
				this->_protocol = rhs._protocol;
				this->_status_code = rhs._status_code;
				this->_status_text = rhs._status_text;
				this->_headers = rhs._headers;
				this->_body = rhs._body;
				this->_response = rhs._response;
				this->_serv = rhs._serv;
				this->_req = rhs._req;
			}
			return *this;
		}

		std::string	construct_response();
		void		build_response();
		void		handle_post();
		void		handle_delete();

		void		print() const;
		int			check_method();
		int			check_redirection();
		int			check_max_body_size();
		int			check_url_exists();
		std::string	get_location();
		std::string build_ressource_path();
		void		simple_response(int code, std::string status);
		void		simple_response(int code, std::string status, std::string path);
		void		directory_response();
		std::string	get_response();


	private:
		std::string							_protocol;
		int									_status_code;
		std::string							_status_text;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		std::string							_response;
		ServerConfig						_serv;
		HttpRequest							_req;



};

#endif
