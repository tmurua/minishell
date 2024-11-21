/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:23:19 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/21 13:31:58 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include "../../include/minishell.h"

// process for single command execution should be placed in a different file
char	*find_executable_path(char *str, char **directories, t_minishell *shell);
char	**create_directories(t_minishell *shell);
char	*build_command_path(char *str, t_minishell *shell);
int		count_arg_tokens(t_token *tokens);
void	add_infile_to_cmd(t_command *cmd, char *filename, t_minishell *shell);
void	add_outfile_to_cmd(t_command *cmd, char *filename, t_minishell *shell);

void	run_program(t_command *cmd) // better naming
{
	int		pid;
	int		status;
	t_files	*infile;
	t_files	*outfile;

	infile = cmd->infile;
	while (infile && infile->next)
		infile = infile->next;
	outfile = cmd->outfile;
	while (outfile && outfile->next)
		outfile = outfile->next;
	pid = fork();
	if (pid == 0)
	{
		reset_signal_handlers();
		if (infile)
		{
			if (dup2(infile->fd, STDIN_FILENO) == -1)
			{
				perror("dup2 infile");
				exit(EXIT_FAILURE);
			}
		}
		if (outfile)
		{
			if (dup2(outfile->fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 outfile");
				exit(EXIT_FAILURE);
			}
		}
		if (execve(cmd->path, cmd->args, *cmd->env) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	ignore_signal_handlers();
	if (waitpid(pid, &status, 0) == -1)
		perror("minishell: waitpid");
	setup_prompt_signals();
}

void	init_command(t_command *cmd, t_token *tokens, t_minishell *shell)
{
	int	arg_count;
	int	i;

	cmd->cmd_name = NULL;
	cmd->path = NULL;
	cmd->env = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	arg_count = count_arg_tokens(tokens);
	i = 0;
	cmd->args = gc_calloc(&shell->gc_head, arg_count + 1, sizeof(char *));
	if (!cmd->args)
		exit(EXIT_FAILURE);
	while (tokens)
	{
		if (tokens->type == TOKEN_BUILTIN_CMD)
		{
			cmd->cmd_name = gc_strdup(&shell->gc_head, tokens->value);
			if (!cmd->cmd_name)
				exit(EXIT_FAILURE);
			cmd->args[i] = gc_strdup(&shell->gc_head, tokens->value);
			if (!cmd->args[i++])
				exit(EXIT_FAILURE);
		}
		if (tokens->type == TOKEN_EXTERN_CMD)
		{
			cmd->cmd_name = gc_strdup(&shell->gc_head, tokens->value);
			if (!cmd->cmd_name)
				exit(EXIT_FAILURE);
			cmd->args[i] = gc_strdup(&shell->gc_head, tokens->value);
			if (!cmd->args[i++])
				exit(EXIT_FAILURE);
			cmd->path = build_command_path(cmd->cmd_name, shell);
		}
		if (tokens->type == TOKEN_ARGUMENT)
		{
			cmd->args[i] = gc_strdup(&shell->gc_head, tokens->value);
			if (!cmd->args[i++])
				exit(EXIT_FAILURE);
		}
		if (tokens->type == TOKEN_REDIRECT_IN)
		{
			if (tokens->next->type == TOKEN_FILENAME)
			{
				add_infile_to_cmd(cmd, tokens->next->value, shell);
				tokens = tokens->next;
			}
		}
		// heredocs
		if (tokens->type == TOKEN_REDIRECT_OUT)
		{
			if (tokens->next->type == TOKEN_FILENAME)
			{
				add_outfile_to_cmd(cmd, tokens->next->value, shell, 0);
				tokens = tokens->next;
			}
		}
		if (tokens->type == TOKEN_REDIRECT_APPEND)
		{
			if (tokens->next->type == TOKEN_FILENAME)
			{
				add_outfile_to_cmd(cmd, tokens->next->value, 1);
				tokens = tokens->next;
			}
		}
		//append
		tokens = tokens->next;
	}
	cmd->args[i] = NULL;
}

// redirection still needs to be implemented
int	init_pipe(t_ast_node *node, t_minishell *shell)
{
	int		fds[2];
	pid_t	pids[2];
	int		status[2];

	pipe(fds);
	pids[0] = fork();
	if (pids[0] == 0)
	{
		close(fds[0]);
		if (dup2(fds[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(fds[1]);
		read_tree(node->left, shell);
		exit(EXIT_SUCCESS);
	}
	pids[1] = fork();
	if (pids[1] == 0)
	{
		close(fds[1]);
		if (dup2(fds[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(fds[0]);
		read_tree(node->right, shell);
		exit(EXIT_SUCCESS);
	}
	close(fds[0]);
	close(fds[1]);
	waitpid(pids[0], &status[0], 0);
	waitpid(pids[1], &status[1], 0);
	return (0);
}

char	*find_executable_path(char *str, char **directories, t_minishell *shell)
{
	int		i;
	char	*full_process;
	char	*full_path;

	i = 0;
	while (directories[i])
	{
		full_process = gc_strjoin(&shell->gc_head, "/", str);
		full_path = gc_strjoin(&shell->gc_head, directories[i], full_process);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

char	**create_directories(t_minishell *shell)
{
	int		i;
	char	**directories;

	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp("PATH=", shell->env[i], 5) == 0)
		{
			directories = ft_split(ft_strchr(shell->env[i], '/'), ':');
			if (!directories)
				exit(EXIT_FAILURE);
			return (directories);
		}
		i++;
	}
	return (NULL);
}

char	*build_command_path(char *str, t_minishell *shell)
{
	char	**directories;
	char	*result_path;

	if (access(str, X_OK) == 0)
		return (gc_strdup(&shell->gc_head, str));
	directories = create_directories(shell);
	if (!directories)
		exit(EXIT_FAILURE);
	result_path = find_executable_path(str, directories, shell);
	free_arguments(directories); // need better naming for function
	return (result_path);
}

int	count_arg_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_ARGUMENT)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

void	add_infile_to_cmd(t_command *cmd, char *filename, t_minishell *shell)
{
	t_files	*new_infile;
	t_files	*current;

	new_infile = gc_calloc(&shell->gc_head, 1, sizeof(t_files));
	if (!new_infile)
		exit(EXIT_FAILURE);
	new_infile->delim = NULL;
	new_infile->next = NULL;
	new_infile->fd = open(filename, O_RDONLY, 0644);
	if (new_infile->fd < 0)
	{
		perror(filename);
		new_infile->fd = open("/dev/null", O_RDONLY);
	}
	if (!cmd->infile)
		cmd->infile = new_infile;
	else
	{
		current = cmd->infile;
		while (current->next)
			current = current->next;
		current->next = new_infile;
	}
}

void	update_filename_tokens(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_REDIRECT_IN
			|| tokens->type == TOKEN_REDIRECT_OUT
			|| tokens->type == TOKEN_REDIRECT_APPEND)
			tokens->next->type = TOKEN_FILENAME;
		tokens = tokens->next;
	}
}

void	add_outfile_to_cmd(t_command *cmd, char *filename, t_minishell *shell, int append_flag)
{
	t_files	*new_outfile;
	t_files	*current;

	new_outfile = gc_calloc(&shell->gc_head, 1, sizeof(t_files));
	if (!new_outfile)
		exit(EXIT_FAILURE);
	new_outfile->delim = NULL;
	new_outfile->next = NULL;
	if (append_flag)
		new_outfile->fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		new_outfile->fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (new_outfile->fd < 0)
	{
		perror(filename);
		new_outfile->fd = open("/dev/null", O_WRONLY);
	}
	if (!cmd->outfile)
		cmd->outfile = new_outfile;
	else
	{
		current = cmd->outfile;
		while (current->next)
			current = current->next;
		current->next = new_outfile;
	}
}
