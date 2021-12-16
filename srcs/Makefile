NAME		=	webserv.out

SRCS		=	server_main.cpp	\

CPP			=	clang++

CPPFLAGS		+=	-Wall -Wextra -Wextra -std=c++98

%.o: %.cpp
				$(CPP) $(CPPFLAGS) -c $<

OBJS		=	$(SRCS:.cpp=.o)


all			:	$(NAME)

$(NAME)		:	$(OBJS)
				$(CPP) -o $(NAME) $(CPPFLAGS) $(OBJS)

san			:	$(OBJS)
				$(CPP) -o $(NAME) -fsanitize=address $(CPPFLAGS) $(OBJS)

clean		:
				rm -rf $(OBJS)

fclean		:	clean
				rm -f $(NAME)

re			:	fclean all