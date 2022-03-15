# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cduvivie <cduvivie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/15 15:16:05 by cduvivie          #+#    #+#              #
#    Updated: 2022/03/15 15:57:57 by cduvivie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SERVER_NAME		=	webserv

SRCS_SERVER		=	main_server.cpp	\
					Socket.cpp Server.cpp \
					HttpRequest.cpp \
					HttpResponse.cpp \
					utils.cpp \
					ServerConfig.cpp\
					Cgi.cpp\
					ConfigFile.cpp\
					Cluster.cpp\

CPP			=	clang++

CPPFLAGS		+=	-Wall -Wextra -Wextra -std=c++98

%.o: %.cpp
				$(CPP) $(CPPFLAGS) -c $<

OBJS_SERVER		=	$(SRCS_SERVER:.cpp=.o)

# defaults to server compilation
all				:	server
server			: 	$(SERVER_NAME)
$(SERVER_NAME)	:	$(OBJS_SERVER)
				$(CPP) -o $(SERVER_NAME) $(CPPFLAGS) $(OBJS_SERVER)

san				:	$(OBJS_SERVER)
					$(CPP) -o $(SERVER_NAME) -fsanitize=address $(CPPFLAGS) $(OBJS_SERVER)

clean			:
					rm -rf $(OBJS_SERVER)

fclean			:	clean
					rm -f $(SERVER_NAME)

re				:	fclean all

.PHONY: all server san clean fclean re