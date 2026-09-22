NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g

SRC = \
	init.c \
	main.c \
	operations.c \
	parser.c \
	threads.c \
	validator.c \
	utils.c \

OBJ = $(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)



%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -g

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
