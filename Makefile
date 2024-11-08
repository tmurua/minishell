NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

LIBFT = ./libft/libft.a

SRC =	main.c \
		input_handling.c \
		lexer.c \
		lexer_state_handler.c \
		lexer_utils.c \
		parser.c \
		signal_handling.c \
		execute_commands.c \
		builtin_commands.c \
		builtin_commands2.c \
		external_commands.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	make -C ./libft --no-print-directory
	echo "libft compiled"
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline
	echo "$(NAME) generated"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make clean -C ./libft --no-print-directory
	rm -f $(OBJ)
	echo "object files deleted"

fclean: clean
	make fclean -C ./libft --no-print-directory
	rm -f $(NAME)
	echo "$(NAME) deleted"

re: fclean all

.PHONY: all clean fclean re

.SILENT:
