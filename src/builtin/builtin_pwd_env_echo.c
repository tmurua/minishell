/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd_env_echo.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:33:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/03 17:27:25 by tmurua           ###   ########.fr       */
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

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	newline;
	int	output_fd;

	i = 1;
	newline = 1;
	if (cmd->args[1] != NULL && ft_strncmp(cmd->args[1], "-n", 3) == 0)
	{
		newline = 0;
		i++;
	}
	output_fd = set_output_fd(cmd->outfile);
	while (cmd->args[i] != NULL)
	{
		ft_putstr_fd(cmd->args[i], output_fd);
		if (cmd->args[i + 1] != NULL)
			ft_putstr_fd(" ", output_fd);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", output_fd);
	return (0);
}
