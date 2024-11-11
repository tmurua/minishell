/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:08:10 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/11 14:56:32 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* libraries */
# include "./libft/libft.h"
/* printf, perror */
# include <stdio.h>
/* malloc, free, exit, getenv */
# include <stdlib.h>
/* readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay */
# include <readline/readline.h>
/* add_history */
# include <readline/history.h>
/* strerror */
# include <string.h>
/*	read, write, access, close, unlink,
	fork, execve, getpid, getppid,
	isatty, ttyname, ttyslot
	getcwd, chdir
	dup, dup2, pipe	*/
# include <unistd.h>
/* open */
# include <fcntl.h>
/* opendir, readdir, closedir */
# include <dirent.h>
/* fork, execve */
# include <sys/types.h>
/* wait, waitpid, wait3, wait4 */
# include <sys/wait.h>
/* stat, lstat, fstat */
# include <sys/stat.h>
/* ioctl */
# include <sys/ioctl.h>
/* signal, sigaction, sigemptyset, sigaddset, kill */
# include <signal.h>
/* tcsetattr, tcgetattr */
# include <termios.h>
/* tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs */
# include <curses.h>
/* perror */
# include <errno.h>
/* PATH_MAX */
# include <limits.h>
/* macros*/
# define TOKEN_COMPLETE	1
# define TOKEN_CONTINUE	0
# define TOKEN_ERROR	-1

/* structures */
/* enumerate all possible token types in minishell */
typedef enum e_token_type
{
	INVALID_TOKEN,//	<-- would delete all 4
	ARGUMENT,
	BUILTIN_CMD,
	SYMBOL,

	TOKEN_BI_ECHO,
	TOKEN_EXEC,
	TOKEN_ARGUMENT,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
}	t_token_type;

typedef enum s_node_type // needs to be renamed e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_BUILTIN, // would delete
}	t_node_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/* enumerate 3 possible states for minishell (state machine approach) */
typedef enum e_lexer_state
{
	DEFAULT_STATE,
	SINGLE_QUOTE_STATE,
	DOUBLE_QUOTE_STATE,
}	t_lexer_state;


typedef struct s_lexer
{
	const char		*str;
	size_t			pos;
	char			current_char;
	t_lexer_state	state;
}	t_lexer;


typedef struct s_command_node
{
	char	*cmd_name;
	char	**args;
}	t_command_node;

typedef struct s_ast_node
{
	t_token				*tokens;
	t_node_type			type;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

/* function prototypes */
/* input_handling.c */
void		main_input_loop(int ac, char **av, char **envp);
void		handle_multiple_args(int ac, char **av);
char		*read_user_input(void);
void		cleanup_input(t_token *tokens, char *input);

/* lexer.c */
t_token		*run_lexer(char *str);
t_lexer		init_lexer(const char *arg);
void		skip_whitespace(t_lexer *lexer);
void		advance_lexer_char(t_lexer *lexer);
t_token		*get_next_token(t_lexer *lexer);

/* lexer_state_handler.c */
char		*collect_token(t_lexer *lexer);
int			handle_default_state(t_lexer *lexer, char **buffer);
int			handle_single_quote_state(t_lexer *lexer, char **buffer);
int			handle_double_quote_state(t_lexer *lexer, char **buffer);
int			advance_and_append(t_lexer *lexer, char **buffer);

/* lexer_utils.c */
char		*ft_strjoin_free(char *s1, const char *s2);
int			count_tokens(t_token *tokens);
t_token		*create_token(t_token_type type, char *value);
void		free_tokens(t_token *tokens);
void		token_to_list(t_token **tokens, t_token **current, t_token *new);

/* variable_expansion.c */
int			handle_variable_expansion(t_lexer *lexer, char **buffer);
char		*collect_variable_name(t_lexer *lexer);
int			get_variable_name_length(const char *str);
char		*get_variable_value(const char *var_name);
int			append_to_buffer(char **buffer, const char *str);

/* parser.c */
t_ast_node	*parse_expression(t_token **current_token,
				int precedence_threshold);

/* interpreter.c */
void		read_tree(t_ast_node root);

/* signal_handling.c */
void		setup_prompt_signals(void);
void		setup_sigint_handler(void);
void		handle_sigint_at_prompt(int sig);
void		setup_sigquit_handler(void);
void		reset_signal_handlers(void);
void		ignore_signal_handlers(void);

/* execute_commands.c */
void		execute_command(t_token *tokens, char **envp);
char		**tokens_to_args(t_token *tokens);
char		**allocate_args_array(int count);
int			copy_token_values(t_token *tokens, char **args);
void		free_arguments(char **args);

/* builtin_commands.c */
int			handle_exit_command(char *input);
void		print_builtin_error(char *command, char *message);
int			too_many_arguments(char **args);
int			is_builtin_command(const char *word);
int			execute_builtin(char **args, char **envp);

/* builtin_commands2.c */
int			builtin_cd(char **args);
int			builtin_pwd(char **args);
int			builtin_env(char **args, char **envp);

/* external_commands.c */
void		execute_external_cmd(char **cmd_args, char **envp, t_token *tokens);
pid_t		fork_child_process(void);
void		execute_in_child(char **cmd_and_args, char **envp, t_token *tokens);
void		handle_parent_process(pid_t child_pid);

#endif
