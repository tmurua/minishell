/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:33:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/11 19:23:22 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
getcwd() sets current working directory path into a path_buffer of size PATH_MAX
then	ft_putendl_fd(void) outputs the current working directory path */
int	builtin_pwd(t_command *cmd)
{
	char	*current_working_directory;
	char	path_buffer[PATH_MAX];
	int		output_fd;

	if (too_many_arguments(cmd->args))
		return (1);
	current_working_directory = getcwd(path_buffer, PATH_MAX);
	if (current_working_directory == NULL)
	{
		perror("minishell: pwd");
		return (1);
	}
	output_fd = set_output_fd(cmd->outfile);
	ft_putendl_fd(current_working_directory, output_fd);
	return (0);
}

int	builtin_env(t_command *cmd, t_minishell *shell)
{
	int	i;
	int	output_fd;

	if (too_many_arguments(cmd->args))
		return (1);
	output_fd = set_output_fd(cmd->outfile);
	i = 0;
	while (shell->env[i] != NULL)
	{
		ft_putendl_fd(shell->env[i], output_fd);
		i++;
	}
	return (0);
}

int	too_many_arguments(char **args)
{
	if (args[1] != NULL)
	{
		ft_putstr_fd("pwd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

/* print all environment variables prefixed with 'declare -x' */
int	print_export_without_args(t_minishell *shell)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(shell->env[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	return (0);
}
