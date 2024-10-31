/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:53:43 by tmurua            #+#    #+#             */
/*   Updated: 2024/10/31 14:21:39 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_exit_command(char *input)
{
	if (input == NULL || !ft_strncmp(input, "exit", 5))
	{
		free(input);
		rl_clear_history();
		exit (0);
	}
	return (0);
}

/* print error messages for built-in commands */
void	print_builtin_error(char *command, char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

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

	if (args[1] != NULL)
	{
		ft_putstr_fd("pwd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	current_working_directory = getcwd(path_buffer, PATH_MAX);
	if (current_working_directory == NULL)
	{
		perror("minishell: pwd");
		return (1);
	}
	ft_putendl_fd(current_working_directory, STDOUT_FILENO);
	return (0);
}

/* dispatches and executes the appropriate built-in command */
int	execute_builtin(char **args, char **envp)
{
	if (args[0] == NULL)
		return (-1);
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (builtin_cd(args));
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (builtin_pwd(args));
	//else if (ft_strncmp(args[0], "echo", 5) == 0)
	//	return (builtin_echo(args));
	//else if (ft_strncmp(args[0], "env", 4) == 0)
	//	return (builtin_env(*envp));
	//else if (ft_strncmp(args[0], "export", 7) == 0)
	//	return (builtin_export(args, envp));
	//else if (ft_strncmp(args[0], "unset", 6) == 0)
	//	return (builtin_unset(args));
	(void)envp;
	return (-1);
}
