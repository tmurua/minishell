/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:53:43 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/25 21:04:47 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_exit_command(char *input, t_minishell *shell)
{
	if (input == NULL || !ft_strncmp(input, "exit", 5))
	{
		free(input);
		rl_clear_history();
		printf("exit\n");
		gc_free_all(shell->gc_head);
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

int	too_many_arguments(char **args)
{
	if (args[1] != NULL)
	{
		ft_putstr_fd("pwd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

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

/* dispatches and executes the appropriate built-in command */
int	execute_builtin(t_minishell *shell)
{
	if (shell->cmd->args[0] == NULL)
		return (-1);
	if (ft_strncmp(shell->cmd->args[0], "cd", 3) == 0)
		return (builtin_cd(shell->cmd->args));
	else if (ft_strncmp(shell->cmd->args[0], "pwd", 4) == 0)
		return (builtin_pwd(shell->cmd));
	else if (ft_strncmp(shell->cmd->args[0], "env", 4) == 0)
		return (builtin_env(shell->cmd, shell));
	else if (ft_strncmp(shell->cmd->args[0], "export", 7) == 0)
		return (builtin_export(shell->cmd->args, shell));
	else if (ft_strncmp(shell->cmd->args[0], "unset", 6) == 0)
		return (builtin_unset(shell->cmd->args, shell));
	else if (ft_strncmp(shell->cmd->args[0], "echo", 5) == 0)
		return (builtin_echo(shell->cmd));
	return (-1);
}
