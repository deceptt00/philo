CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread
LIB = -lpthread
SRC = main.c utils.c parsing.c actions.c simulation.c
OBJ = $(SRC:.c=.o)
DEP = philo.h

NAME = philo

all: $(NAME)

$(OBJ): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIB)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

.SECONDARY: 