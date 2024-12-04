/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:08:10 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/03 17:34:38 by tmurua           ###   ########.fr       */
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
# define MIN_PRECEDENCE_LVL 0

/* structures */
typedef struct s_token		t_token;
typedef struct s_ast_node	t_ast_node;
typedef struct s_command	t_command;
typedef struct s_files		t_files;

/* context structure to group data together and improve code organization */
typedef struct s_minishell
{
	char				**env;
	int					last_exit_status;
	t_list				*gc_head;
	t_token				*tokens;
	t_ast_node			*ast_root;
	t_command			*cmd;
	t_list				*heredocs;
	// int					heredoc_flag;
	// t_lexer_state		state;
	// int					fd[2];
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
	TOKEN_HEREDOC_DELIMITER,
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
	int					heredoc_quote;
	struct s_files		*next;
}						t_files;

typedef struct s_command
{
	t_files				*infile;
	t_files				*outfile;
	char				*cmd_name;
	char				*path;
	char				**args;
	int					has_heredoc;
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
void					handle_multiple_args(int ac, char **av,
							t_minishell *shell);
char					*read_user_input(void);

/* lexer.c */
void					run_lexer(char *str, t_minishell *shell);
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
void					token_to_list(t_token **tokens, t_token **current,
							t_token *new);
void					update_redirect_tokens(t_token *tokens);

/* variable_expansion.c */
int						handle_variable_expansion(t_lexer *lexer, char **buffer,
							t_minishell *shell);
char					*collect_variable_name(t_lexer *lexer,
							t_minishell *shell);
int						get_variable_name_length(const char *str);
char					*get_variable_value(const char *var_name,
							t_minishell *shell);
int						append_to_buffer(char **buffer, const char *str,
							t_minishell *shell);

/* parser.c */
t_ast_node				*parse_expression(t_minishell *shell,
							int precedence_threshold);

/* heredoc.c */
void					init_heredoc(t_minishell *shell, t_token *token);
void					close_heredoc_list(t_minishell *shell);

/* heredoc2.c */
void					heredoc_scan(char *input, t_minishell *shell);

/* interpreter.c */
void					read_tree(t_ast_node *root, t_minishell *shell);
int						evaluate_and_execute(t_ast_node *node,
							t_minishell *shell);
int						execute_command_node(t_ast_node *node,
							t_minishell *shell);
int						execute_logical_operator_node(t_ast_node *node,
							t_minishell *shell);

/* prompt_signals.c */
void					setup_prompt_signals(t_minishell *shell);
void					setup_sigint_handler(t_minishell *shell);
void					handle_sigint_at_prompt(int sig);
void					setup_sigquit_handler(t_minishell *shell);

/* child_signals.c */
void					reset_signal_handlers(t_minishell *shell);
void					ignore_signal_handlers(t_minishell *shell);

/* execute_commands.c */
void					init_command(t_command *cmd, t_token *node_tokens,
							t_minishell *shell);
void					initialize_command_struct(t_command *cmd);
int						count_arg_tokens(t_token *tokens);
int						process_argument(t_command *cmd, t_token *token,
							t_minishell *shell, int i);
t_token					*process_token(t_command *cmd, t_token *token,
							t_minishell *shell, int *i);

/* process_command_tokens.c */
int						process_builtin_cmd(t_command *cmd, t_token *token,
							t_minishell *shell, int i);
int						process_extern_cmd(t_command *cmd, t_token *token,
							t_minishell *shell, int i);
t_token					*process_redirect_in(t_command *cmd, t_token *token,
							t_minishell *shell);
t_token					*process_redirect_out(t_command *cmd, t_token *token,
							t_minishell *shell);
t_token					*process_redirect_append(t_command *cmd, t_token *token,
							t_minishell *shell);

/* command_files.c */
t_files					*create_new_file_node(t_minishell *shell);
void					append_file_node(t_files **file_list,
							t_files *new_file);
void					add_infile_to_cmd(t_command *cmd, char *filename,
							t_minishell *shell);
void					add_outfile_to_cmd(t_command *cmd, char *filename,
							t_minishell *shell, int append_flag);

/* path_builder.c */
char					*build_command_path(char *str, t_minishell *shell);

/* execute_external.c */
void					execute_external(t_command *cmd, char **env,
							t_minishell *shell);
void					handle_command_not_found(const char *cmd_name,
							t_minishell *shell);
void					fork_and_execute(t_command *cmd, char **env,
							t_minishell *shell);

/* process_execution.c */
void					execute_command_child(t_command *cmd, char **env,
							t_minishell *shell);
void					handle_parent_process(pid_t pid, t_minishell *shell);

/* process_redirections.c */
t_files					*get_last_file(t_files *files);
void					setup_redirections(t_files *infile, t_files *outfile,
							t_minishell *shell);

/* builtin_handler.c */
int						is_builtin_command(const char *word);
int						execute_builtin(t_command *cmd, t_minishell *shell);
int						set_output_fd(t_files *outfile);
void					print_builtin_error(char *command, char *message);
int						too_many_arguments(char **args);

/* builtin_pwd_env_echo.c */
int						builtin_pwd(t_command *cmd);
int						builtin_env(t_command *cmd, t_minishell *shell);
int						builtin_echo(t_command *cmd);

/* builtin_exit.c */
int						handle_exit_command(char *input, t_minishell *shell);
int						handle_exit_with_arguments(char **args,
							t_minishell *shell);
int						handle_numeric_argument(char *arg);
int						is_numeric_argument(const char *arg);

/* builtin_cd.c */
int						builtin_cd(char **args, t_minishell *shell);
int						get_current_directory(char *cwd);
int						get_target_path(char **args, t_minishell *shell,
							char **path);
int						change_directory(char *path);
int						update_environment(char *oldpwd, t_minishell *shell);

/* builtin_export.c */
int						builtin_export(char **args, t_minishell *shell);
int						process_export_argument(const char *arg,
							t_minishell *shell);
int						set_env_variable(const char *name, const char *value,
							t_minishell *shell);
char					*create_env_string(const char *name, const char *value,
							t_minishell *shell);
int						add_env_variable(char *new_var, t_minishell *shell);

/* builtin_unset.c */
int						builtin_unset(char **args, t_minishell *shell);
int						process_unset_argument(const char *arg,
							t_minishell *shell);
int						is_valid_env_name(const char *name);
int						unset_env_variable(const char *name,
							t_minishell *shell);
int						find_env_index(const char *name, t_minishell *shell);

/* environment_utils.c */
char					**duplicate_env(char **envp, t_minishell *shell);

/* pipe.c */
int						init_pipe(t_ast_node *node, t_minishell *shell);

#endif
