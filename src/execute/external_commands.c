/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 08:57:29 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/27 00:27:09 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_external(t_command *cmd, char **env, t_minishell *shell)
{
	int		pid;
	int		status;
	t_files	*infile;
	t_files	*outfile;

	infile = cmd->infile;
	while (infile && infile->next)
		infile = infile->next;
	outfile = cmd->outfile;
	while (outfile && outfile->next)
		outfile = outfile->next;
	pid = fork();
	if (pid == 0)
	{
		reset_signal_handlers(shell);
		if (infile)
		{
			if (dup2(infile->fd, STDIN_FILENO) == -1)
			{
				perror("dup2 infile");
				gc_free_all(shell->gc_head);
				exit(EXIT_FAILURE);
			}
			if (cmd->has_heredoc)
				close_heredoc_list(shell);
		}
		if (outfile)
		{
			if (dup2(outfile->fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 outfile");
				gc_free_all(shell->gc_head);
				exit(EXIT_FAILURE);
			}
		}
		if (execve(cmd->path, cmd->args, env) == -1)
		{
			perror("execve");
			gc_free_all(shell->gc_head);
			exit(EXIT_FAILURE);
		}
	}
	ignore_signal_handlers(shell);
	if (waitpid(pid, &status, 0) == -1)
		perror("minishell: waitpid");
	setup_prompt_signals(shell);
}


