NAME	= minishell

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g -I include

LIBFT	= ./libft/libft.a

# Source Files
SRC		= \
	./src/main.c \
	./src/environment/environment_utils.c \
	./src/environment/variable_expansion.c \
	./src/input/input_handling.c \
	./src/lexer/lexer.c \
	./src/lexer/lexer_assign_types.c \
	./src/lexer/lexer_state_handler.c \
	./src/lexer/lexer_utils.c \
	./src/parser/parser.c \
	./src/parser/heredoc.c \
	./src/interpreter/interpreter.c \
	./src/pipe/pipe.c \
	./src/builtin/builtin_handler.c \
	./src/builtin/builtin_exit_pwd_env_echo.c \
	./src/builtin/builtin_cd.c \
	./src/builtin/builtin_export.c \
	./src/builtin/builtin_unset.c \
	./src/execute/execute_commands.c \
	./src/execute/external_commands.c \
	./src/execute/path_builder.c \
	./src/signal/signal_handling.c

OBJ		= $(SRC:.c=.o)

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
