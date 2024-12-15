/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:53:43 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/15 21:25:38 by dlemaire         ###   ########.fr       */
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

int	execute_builtin(t_command *cmd, t_minishell *shell)
{
	int	status;

	status = -1;
	if (cmd->args[0] == NULL)
		return (-1);
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		status = builtin_cd(cmd->args, shell);
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		status = builtin_pwd(cmd);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		status = builtin_env(cmd, shell);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		status = builtin_export(cmd->args, shell);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		status = builtin_unset(cmd->args, shell);
	else if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
	{
		if (cmd->infile && cmd->redirect_error_flag == 1)
			status = 1;
		else
			status = builtin_echo(cmd);
	}
	else
		return (-100);
	shell->last_exit_status = status;
	return (status);
}

// int	set_output_fd(t_files *outfile)
// {
// 	if (!outfile)
// 		return (STDOUT_FILENO);
// 	else
// 		return (outfile->fd);
// }

/* print error messages for built-in commands */
void	print_builtin_error(char *command, char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	handle_exit_if_requested(char *input, t_minishell *shell)
{
	if (handle_exit_command(input, shell))
	{
		free(input);
		return (1);
	}
	return (0);
}
