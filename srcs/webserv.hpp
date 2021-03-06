/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduvivie <cduvivie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 15:16:38 by cduvivie          #+#    #+#             */
/*   Updated: 2022/03/15 15:16:38 by cduvivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# define PUBLIC_HTML_FOLDER "public_html"

/* Macro to set the appropriate Pyhton3 interpreter depending on the OS */
# ifndef PYTHON_INTERPRETER
#  ifdef __linux__  //Linux
#   define PYTHON_INTERPRETER "/usr/bin/python3.7"
#  endif
#  ifdef __APPLE__  //MAC OS
#   define PYTHON_INTERPRETER "/usr/local/bin/python3"
#  endif
# endif

#endif
