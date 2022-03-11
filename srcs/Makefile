SERVER_NAME		=	server.out
CLIENT_NAME		=	client.out

SRCS_SERVER		=	main_server.cpp	\
					SimpleSocket.cpp ServerSocket.cpp \
					HttpRequest.cpp \
					HttpResponse.cpp \
					utils.cpp \
					ServerConfig.cpp\
					Cgi.cpp\
					ConfigFile.cpp\
					Cluster.cpp\

SRCS_CLIENT		=	main_client.cpp	\
					SimpleSocket.cpp ClientSocket.cpp \
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

client			:	$(CLIENT_NAME)
$(CLIENT_NAME)	:	$(OBJS_CLIENT)
				$(CPP) -o $(CLIENT_NAME) $(CPPFLAGS) $(OBJS_CLIENT)

san				:	$(OBJS_SERVER)
					$(CPP) -o $(SERVER_NAME) -fsanitize=address $(CPPFLAGS) $(OBJS_SERVER)

clean			:
					rm -rf $(OBJS_SERVER) $(OBJS_CLIENT)

fclean			:	clean
					rm -f $(SERVER_NAME) $(CLIENT_NAME)

re				:	fclean all
