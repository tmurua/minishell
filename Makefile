NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

LIBFT = ./libft/libft.a

SRC =	main.c \
		input_handling.c \
		lexer.c \
		lexer_assign_types.c \
		interpreter.c \
		lexer_state_handler.c \
		lexer_utils.c \
		variable_expansion.c \
		parser.c \
		pipe.c \
		signal_handling.c \
		execute_commands.c \
		builtin_commands.c \
		builtin_commands2.c \
		builtin_cmd_export.c \
		builtin_cmd_unset.c \
		environment_utils.c \
		external_commands.c

OBJ_DIRECTORY = build
OBJ = $(addprefix $(OBJ_DIRECTORY)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	make -C ./libft --no-print-directory
	echo "libft compiled"
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline
	echo "$(NAME) generated"

$(OBJ_DIRECTORY)/%.o: %.c | $(OBJ_DIRECTORY)
	$(CC) $(CFLAGS) -c $< -o $@


$(OBJ_DIRECTORY):
	mkdir -p $(OBJ_DIRECTORY)

clean:
	make clean -C ./libft --no-print-directory
	rm -f $(OBJ)
	echo "object files deleted"

fclean: clean
	make fclean -C ./libft --no-print-directory
	rm -f $(NAME)
	rm -rf $(OBJ_DIRECTORY)
	echo "$(NAME) deleted"

re: fclean all

.PHONY: all clean fclean re

.SILENT:
