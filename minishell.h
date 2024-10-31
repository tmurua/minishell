/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:08:10 by tmurua            #+#    #+#             */
/*   Updated: 2024/10/31 13:13:44 by tmurua           ###   ########.fr       */
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
}	t_ast_node;

/* function prototypes */
/* input_handling.c */
void		main_input_loop(int ac, char **av, char **envp);
void		handle_multiple_args(int ac, char **av);
char		*read_user_input(void);
void		execute_command(char **args, char **envp);
void		cleanup_input(char **args, t_token *tokens, char *input);

/* signal_handling.c */
void		setup_signal_handler(void);
void		handle_sigint(int sig);

/* lexer.c */
t_token		*run_lexer(char *str);
void		free_tokens(t_token *tokens);

/* ast.c */
t_ast_node	*parse_tokens(t_token *tokens);

/* builtin_commands.c */
int			handle_exit_command(char *input);
void		print_builtin_error(char *command, char *message);
int			builtin_cd(char **args);
int			builtin_echo(char **args);
int			execute_builtin(char **args, char **envp);

#endif
