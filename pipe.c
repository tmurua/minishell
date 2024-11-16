/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:23:19 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/16 01:07:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include "minishell.h"

// process for single command execution should be placed in a different file
char	*find_executable_path(char *str, char **directories);
char	**create_directories(char *envp[]);
char	*build_command_path(char *str, char *envp[]);
int		count_arg_tokens(t_token *tokens);

void	run_program(t_command *cmd)
{
	int	pid;
	int	source;

	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd->path, cmd->args, cmd->envp) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	waitpid(pid, &source, 0);
}

void	init_command(t_command *cmd, t_token *tokens, char **envp)
{
	int	arg_count;
	int	i;

	cmd->cmd_name = NULL;
	cmd->path = NULL;
	cmd->envp = NULL;
	arg_count = count_arg_tokens(tokens);
	i = 0;
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		exit(EXIT_FAILURE);
	while (tokens)
	{
		if (tokens->type == TOKEN_BUILTIN_CMD)
		{
			cmd->cmd_name = ft_strdup(tokens->value);
			if (!cmd->cmd_name)
				exit(EXIT_FAILURE);
			cmd->args[i] = ft_strdup(tokens->value);
			if (!cmd->args[i++])
				exit(EXIT_FAILURE);
		}
		if (tokens->type == TOKEN_EXTERN_CMD)
		{
			cmd->cmd_name = ft_strdup(tokens->value);
			if (!cmd->cmd_name)
				exit(EXIT_FAILURE);
			cmd->args[i] = ft_strdup(tokens->value);
			if (!cmd->args[i++])
				exit(EXIT_FAILURE);
			cmd->path = build_command_path(cmd->cmd_name, envp);
		}
		if (tokens->type == TOKEN_ARGUMENT)
		{
			cmd->args[i] = ft_strdup(tokens->value);
			if (!cmd->args[i++])
				exit(EXIT_FAILURE);
		}
		if (tokens->type == TOKEN_REDIRECT_IN)
		{
			if (tokens->next->type == TOKEN_FILENAME)
			{
				cmd->infile->fd = open(tokens->next->value, O_RDONLY);
				if (cmd->infile->fd < 0)
				{
					perror(tokens->next->value);
					cmd->infile->fd = open("/dev/null", O_RDONLY);
				}
			}
		}
		if (tokens->type == TOKEN_REDIRECT_OUT)
		{
			if (tokens->next->type == TOKEN_FILENAME)
			{
				cmd->outfile->fd = open(tokens->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (cmd->outfile->fd < 0)
				{
					perror(tokens->next->value);
					cmd->outfile->fd = open("/dev/null", O_WRONLY);
				}
			}
		}
		tokens = tokens->next;
	}
	cmd->args[i] = NULL;
	cmd->envp = envp;
}

// redirection still needs to be implemented
int	init_pipe(t_ast_node *node, char **envp)
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
		read_tree(node->left, envp);
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
		read_tree(node->right, envp);
		exit(EXIT_SUCCESS);
	}
	close(fds[0]);
	close(fds[1]);
	waitpid(pids[0], &status[0], 0);
	waitpid(pids[1], &status[1], 0);
	return (0);
}

char	*find_executable_path(char *str, char **directories)
{
	int		i;
	char	*full_process;
	char	*full_path;

	i = 0;
	while (directories[i])
	{
		full_process = ft_strjoin("/", str);
		full_path = ft_strjoin(directories[i], full_process);
		free(full_process);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	**create_directories(char *envp[])
{
	int		i;
	char	**directories;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
		{
			directories = ft_split(ft_strchr(envp[i], '/'), ':');
			if (!directories)
				exit(EXIT_FAILURE);
			return (directories);
		}
		i++;
	}
	return (NULL);
}

char	*build_command_path(char *str, char *envp[])
{
	char	**directories;
	char	*result_path;

	if (access(str, X_OK) == 0)
		return (ft_strdup(str));
	directories = create_directories(envp);
	if (!directories)
		exit(EXIT_FAILURE);
	result_path = find_executable_path(str, directories);
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