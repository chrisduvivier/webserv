# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cduvivie <cduvivie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/15 15:16:05 by cduvivie          #+#    #+#              #
#    Updated: 2022/03/15 15:19:00 by cduvivie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SERVER_NAME		=	server.out

SRCS_SERVER		=	main_server.cpp	\
					Socket.cpp Server.cpp \
					HttpRequest.cpp \
					HttpResponse.cpp \
					utils.cpp \
					ServerConfig.cpp\
					Cgi.cpp\
					ConfigFile.cpp\
					Cluster.cpp\

SRCS_CLIENT		=	main_client.cpp	\
					Socket.cpp ClientSocket.cpp \
					utils.cpp \

CPP			=	clang++

CPPFLAGS		+=	-Wall -Wextra -Wextra -std=c++98

%.o: %.cpp
				$(CPP) $(CPPFLAGS) -c $<

OBJS_SERVER		=	$(SRCS_SERVER:.cpp=.o)
OBJS_CLIENT		=	$(SRCS_CLIENT:.cpp=.o)

# defaults to server compilation
all				:	server  client
server			: 	$(SERVER_NAME)
$(SERVER_NAME)	:	$(OBJS_SERVER)
				$(CPP) -o $(SERVER_NAME) $(CPPFLAGS) $(OBJS_SERVER)

san				:	$(OBJS_SERVER)
					$(CPP) -o $(SERVER_NAME) -fsanitize=address $(CPPFLAGS) $(OBJS_SERVER)

clean			:
					rm -rf $(OBJS_SERVER) $(OBJS_CLIENT)

fclean			:	clean
					rm -f $(SERVER_NAME)

re				:	fclean all

.PHONY: all server san clean fclean re