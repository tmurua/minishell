/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 08:57:29 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/13 04:32:58 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	check if cmd path exists, handle errors if not or exec cmd by forking a
	child process */
int	execute_external(t_command *cmd, char **env, t_minishell *shell)
{
	if (!cmd->path)
	{
		handle_command_not_found(cmd->cmd_name, shell);
		return (1);
	}
	fork_and_execute(cmd, env, shell);
	return (0);
}

/* print an error message and set last exit status to 127 */
void	handle_command_not_found(const char *cmd_name, t_minishell *shell)
{
	fprintf(stderr, "minishell: %s: command not found\n", cmd_name);
	shell->last_exit_status = 127;
}

/*	fork current process; in child process, exec external cmd;
	in parent process, wait for child and update exit status */
void	fork_and_execute(t_command *cmd, char **env, t_minishell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_command_child(cmd, env, shell);
	else if (pid > 0)
		handle_parent_process(pid, shell);
	else
	{
		perror("minishell: fork");
		shell->last_exit_status = 1;
	}
}
