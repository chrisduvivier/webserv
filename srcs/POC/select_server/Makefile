NAME		=	select_server.out

SRCS		=	lib.cpp					\
				main.cpp				\

CPP			=	clang++

%.o: %.cpp
				$(CPP) $(CPPFLAGS) -c $<

OBJS		=	$(SRCS:.cpp=.o)

CPPFLAGS		+=	-Wall -Wextra

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