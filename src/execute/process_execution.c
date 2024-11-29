/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:56:17 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/29 14:41:59 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	reset signal handlers, setup redirections, exec cmd in child process.
	if execve fails, print an error, free garbage, and exit with status 126 */
void	execute_command_child(t_command *cmd, char **env,
		t_minishell *shell)
{
	t_files	*infile;
	t_files	*outfile;

	reset_signal_handlers(shell);
	infile = get_last_file(cmd->infile);
	outfile = get_last_file(cmd->outfile);
	setup_redirections(infile, outfile, shell);
	if (execve(cmd->path, cmd->args, env) == -1)
	{
		perror("execve");
		gc_free_all(shell->gc_head);
		exit(126);
	}
}

/*	wait for child process to finish, update last exit status based on
	child's exit status or termination signal, and setup prompt signals */
void	handle_parent_process(pid_t pid, t_minishell *shell)
{
	int	status;

	ignore_signal_handlers(shell);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("minishell: waitpid");
		shell->last_exit_status = 1; // Waitpid failed
	}
	else
	{
		if (WIFEXITED(status))
			shell->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->last_exit_status = 128 + WTERMSIG(status);
		else
			shell->last_exit_status = 1; // Unknown failure
	}
	setup_prompt_signals(shell);
}
