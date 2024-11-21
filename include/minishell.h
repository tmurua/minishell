/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:08:10 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/21 13:32:23 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* libraries */
# include "../libft/libft.h"
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
# define TOKEN_COMPLETE 1
# define TOKEN_CONTINUE 0
# define TOKEN_ERROR -1

/* structures */
/* context structure to group data together and improve code organization */
typedef struct s_minishell
{
	char				**env;
	t_list				*gc_head;
	// t_token				*tokens;
	// t_ast_node			*ast_root;
	// t_lexer_state		state;
	// int					fd[2];
	// t_command			*cmd;
	// t_token				*cmd_args;
}						t_minishell;

/* enumerate all possible token types in minishell */
typedef enum e_token_type
{
	TOKEN_INVALID,
	TOKEN_BUILTIN_CMD,
	TOKEN_EXTERN_CMD,
	TOKEN_ARGUMENT,
	TOKEN_ENV_VARIABLE,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_FILENAME,
	TOKEN_AND,
	TOKEN_OR,
}						t_token_type;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_BUILTIN, // would delete
}						t_node_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	struct s_token		*next;
}						t_token;

/* enumerate 3 possible states for minishell */
typedef enum e_lexer_state
{
	DEFAULT_STATE,
	SINGLE_QUOTE_STATE,
	DOUBLE_QUOTE_STATE,
}						t_lexer_state;

typedef struct s_lexer
{
	const char			*str;
	size_t				pos;
	char				current_char;
	t_lexer_state		state;
	int					command_expected;
}						t_lexer;

typedef struct s_files
{
	int					fd;
	char				*delim;
	struct s_files		*next;
}						t_files;

typedef struct s_command
{
	t_files				*infile;
	t_files				*outfile;
	char				*cmd_name;
	char				*path;
	char				**args;
	char				***env;
}						t_command;

typedef struct s_ast_node
{
	t_token				*tokens;
	t_node_type			type;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}						t_ast_node;

/* function prototypes */
/* input_handling.c */
void					main_input_loop(int ac, char **av, t_minishell *shell);
void					handle_multiple_args(int ac, char **av);
char					*read_user_input(void);
void					cleanup_input(t_token *tokens, char *input);

/* lexer.c */
t_token					*run_lexer(char *str, t_minishell *shell);
t_lexer					init_lexer(const char *arg);
void					skip_whitespace(t_lexer *lexer);
void					advance_lexer_char(t_lexer *lexer);
t_token					*get_next_token(t_lexer *lexer, t_minishell *shell);

/* lexer_assign_types.c */
int						is_special_character(t_lexer *lexer);
t_token_type			handle_special_char_token(t_lexer *lexer, char **value,
							t_minishell *shell);
char					*collect_special_character(t_lexer *lexer,
							t_minishell *shell);
t_token_type			get_special_character_token_type(char *value);
t_token_type			handle_regular_token(t_lexer *lexer, char **value,
							t_minishell *shell);

/* lexer_state_handler.c */
char					*collect_token(t_lexer *lexer, t_minishell *shell);
int						handle_default_state(t_lexer *lexer, char **buffer,
							t_minishell *shell);
int						handle_single_quote_state(t_lexer *lexer, char **buffer,
							t_minishell *shell);
int						handle_double_quote_state(t_lexer *lexer, char **buffer,
							t_minishell *shell);
int						advance_and_append(t_lexer *lexer, char **buffer,
							t_minishell *shell);

/* lexer_utils.c */
int						count_tokens(t_token *tokens);
t_token					*create_token(t_token_type type, char *value,
							t_minishell *shell);
void					free_tokens(t_token *tokens);
void					token_to_list(t_token **tokens, t_token **current,
							t_token *new);

/* variable_expansion.c */
int						handle_variable_expansion(t_lexer *lexer, char **buffer,
							t_minishell *shell);
char					*collect_variable_name(t_lexer *lexer,
							t_minishell *shell);
int						get_variable_name_length(const char *str);
char					*get_variable_value(const char *var_name);
int						append_to_buffer(char **buffer, const char *str,
							t_minishell *shell);

/* parser.c */
t_ast_node				*parse_expression(t_token **current_token,
							int precedence_threshold, t_minishell *shell);

/* interpreter.c */
void					read_tree(t_ast_node *root, t_minishell *shell);
int						evaluate_and_execute(t_ast_node *node,
							t_minishell *shell);
void					execute_command_node(t_token *tokens,
							t_minishell *shell);

/* signal_handling.c */
void					setup_prompt_signals(void);
void					setup_sigint_handler(void);
void					handle_sigint_at_prompt(int sig);
void					setup_sigquit_handler(void);
void					reset_signal_handlers(void);
void					ignore_signal_handlers(void);

/* execute_commands.c */
void					execute_command(t_token *tokens, t_minishell *shell);
char					**tokens_to_args(t_token *tokens, t_minishell *shell);
char					**allocate_args_array(int count, t_minishell *shell);
int						copy_token_values(t_token *tokens, char **args,
							t_minishell *shell);
void					free_arguments(char **args);

/* builtin_commands.c */
int						handle_exit_command(char *input, t_minishell *shell);
void					print_builtin_error(char *command, char *message);
int						too_many_arguments(char **args);
int						is_builtin_command(const char *word);
int						execute_builtin(char **args, t_minishell *shell);

/* builtin_commands2.c */
int						builtin_cd(char **args);
int						builtin_pwd(char **args);
int						builtin_env(char **args, t_minishell *shell);
int						builtin_echo(char **args);

/* builtin_cmd_export.c */
int						builtin_export(char **args, t_minishell *shell);
int						process_export_argument(const char *arg,
							t_minishell *shell);
int						is_valid_env_name(const char *name);
int						set_env_variable(const char *name, const char *value,
							t_minishell *shell);
int						find_env_index(const char *name, t_minishell *shell);
char					*create_env_string(const char *name, const char *value,
							t_minishell *shell);
int						add_env_variable(char *new_var, t_minishell *shell);

/* builtin_cmd_unset.c */
int						builtin_unset(char **args, t_minishell *shell);
int						process_unset_argument(const char *arg,
							t_minishell *shell);
int						unset_env_variable(const char *name,
							t_minishell *shell);

/* environment_utils.c */
char					**duplicate_env(char **envp, t_minishell *shell);
void					free_env(char **env);

/* external_commands.c */
void					execute_external_cmd(char **cmd_args,
							t_minishell *shell, t_token *tokens);
pid_t					fork_child_process(void);
void					execute_in_child(char **cmd_and_args,
							t_minishell *shell, t_token *tokens);
void					handle_parent_process(pid_t child_pid);

/* pipe.c */
int						init_pipe(t_ast_node *node, t_minishell *shell);
void					init_command(t_command *cmd, t_token *tokens,
							t_minishell *shell);
void					run_program(t_command *cmd);
void					update_filename_tokens(t_token *tokens);

#endif
