/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:33:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/13 11:54:10 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* chdir() changes directories via the OS, with 'path' as target path
'result' checks success, if result != 0, handle input/errors with perror() */
int	builtin_cd(char **args)
{
	int		result;
	char	*path;

	path = args[1];
	if (path == NULL)
	{
		print_builtin_error("cd", "expected argument");
		return (1);
	}
	result = chdir(path);
	if (result != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

/*
getcwd() sets current working directory path into a path_buffer of size PATH_MAX
then ft_putendl_fd() outputs the current working directory path */
int	builtin_pwd(char **args)
{
	char	*current_working_directory;
	char	path_buffer[PATH_MAX];

	if (too_many_arguments(args))
		return (1);
	current_working_directory = getcwd(path_buffer, PATH_MAX);
	if (current_working_directory == NULL)
	{
		perror("minishell: pwd");
		return (1);
	}
	ft_putendl_fd(current_working_directory, STDOUT_FILENO);
	return (0);
}

/* envp: array of strings representing the environment variables */
int	builtin_env(char **args, char **envp)
{
	int	i;

	if (too_many_arguments(args))
		return (1);
	i = 0;
	while (envp[i] != NULL)
	{
		ft_putendl_fd(envp[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (args[1] != NULL && ft_strncmp(args[1], "-n", 3) == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i] != NULL)
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1] != NULL)
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

