/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:08:10 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/06 11:51:15 by tmurua           ###   ########.fr       */
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

/* structures */
/* enumerates all possible token types in minishell */
typedef enum e_token_type
{
	INVALID_TOKEN,
	ARGUMENT,
	BUILTIN_CMD,
	SYMBOL,
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_lexer
{
	const char	*str;
	size_t		pos;
	char		current_char;
}	t_lexer;

typedef enum s_node_type
{
	NODE_COMMAND,
	NODE_BUILTIN,
}	t_node_type;

typedef struct s_command_node
{
	char	*cmd_name;
	char	**args;
}	t_command_node;

typedef struct s_ast_node
{
	t_node_type			type;
	t_command_node		command; // *union* might be used here later
	struct s_ast_node	*next;
	// left
	// right
}	t_ast_node;

/* function prototypes */
/* input_handling.c */
void		main_input_loop(int ac, char **av, char **envp);
void		handle_multiple_args(int ac, char **av);
char		*read_user_input(void);
void		execute_command(char **args, char **envp);
void		cleanup_input(char **args, t_token *tokens, char *input);

/* signal_handling.c */
void		setup_prompt_signals(void);
void		setup_sigint_handler(void);
void		handle_sigint_at_prompt(int sig);
void		setup_sigquit_handler(void);
void		reset_signal_handlers(void);
void		ignore_signal_handlers(void);

/* external_commands.c */
void		execute_external_cmd(char **cmd_and_args, char **envp);
pid_t		fork_child_process(void);
void		execute_in_child(char **cmd_and_args, char **envp);
void		handle_parent_process(pid_t child_pid);

/* lexer.c */
t_lexer		init_lexer(const char *arg);
void		move_forward(t_lexer *lexer);
t_token		*run_lexer(char *str);

/* get_next_token.c */
t_token		*get_next_token(t_lexer *lexer);
void		skip_whitespace(t_lexer *lexer);
char		*collect_word(t_lexer *lexer);
char		*collect_symbol(t_lexer *lexer);

/* token_utils.c */
t_token		*create_token(t_token_type type, char *value);
void		free_tokens(t_token *tokens);
int			count_tokens(const char *str);

/* parser.c */
t_ast_node	*parse_tokens(t_token *tokens);
int			count_tokens_in_arr(t_token *tokens);

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

#endif
