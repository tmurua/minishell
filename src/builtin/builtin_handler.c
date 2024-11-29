/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:53:43 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/29 13:29:19 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin_command(const char *word)
{
	if (ft_strncmp(word, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(word, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(word, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(word, "export", 7) == 0)
		return (1);
	if (ft_strncmp(word, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(word, "env", 4) == 0)
		return (1);
	if (ft_strncmp(word, "exit", 5) == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_minishell *shell)
{
	int	status;

	status = -1;
	if (shell->cmd->args[0] == NULL)
		return (-1);
	if (ft_strncmp(shell->cmd->args[0], "cd", 3) == 0)
		status = builtin_cd(shell->cmd->args, shell);
	else if (ft_strncmp(shell->cmd->args[0], "pwd", 4) == 0)
		status = builtin_pwd(shell->cmd);
	else if (ft_strncmp(shell->cmd->args[0], "env", 4) == 0)
		status = builtin_env(shell->cmd, shell);
	else if (ft_strncmp(shell->cmd->args[0], "export", 7) == 0)
		status = builtin_export(shell->cmd->args, shell);
	else if (ft_strncmp(shell->cmd->args[0], "unset", 6) == 0)
		status = builtin_unset(shell->cmd->args, shell);
	else if (ft_strncmp(shell->cmd->args[0], "echo", 5) == 0)
		status = builtin_echo(shell->cmd);
	else
		return (-1);
	shell->last_exit_status = status;
	return (status);
}

int	set_output_fd(t_files *outfile)
{
	if (!outfile)
		return (STDOUT_FILENO);
	else
		return (outfile->fd);
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

int	too_many_arguments(char **args)
{
	if (args[1] != NULL)
	{
		ft_putstr_fd("pwd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}
