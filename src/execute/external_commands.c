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


/*	fork new child process with fork_child_process(), if forking works execute
	external command in child process with execute_in_child(), if not run
	handle_parent_process() so parent process waits for child proces to finish*/
// void	execute_external_cmd(char **cmd_args, t_minishell *shell)
// {
// 	pid_t	child_pid;

// 	child_pid = fork_child_process();
// 	if (child_pid < 0)
// 		return ;
// 	if (child_pid == 0)
// 		execute_in_child(cmd_args, shell);
// 	else
// 		handle_parent_process(child_pid, shell);
// }

/*	fork new child process with fork() and return PID of child process */
// pid_t	fork_child_process(void)
// {
// 	pid_t	child_pid;

// 	child_pid = fork();
// 	if (child_pid < 0)
// 		perror("minishell: fork");
// 	return (child_pid);
// }

/*	reset signal handlers to default in child process
	execute external command in child process with excve() */
// void	execute_in_child(char **cmd_and_args, t_minishell *shell)
// {
// 	reset_signal_handlers(shell);
// 	// execve needs the command path to be built
// 	if (execve(cmd_and_args[0], cmd_and_args, shell->env) < 0)
// 	{
// 		perror("minishell: execve");
// 		gc_free_all(shell->gc_head);
// 		exit(EXIT_FAILURE);
// 	}
// }


/*	ignore_signal_handlers to prevent shell interruptions while waiting
	wait for child process to finish, and after is finished
	restore signal handlers with running setup_prompt_signals() */
// void	handle_parent_process(pid_t child_pid, t_minishell *shell)
// {
// 	int	child_status;

// 	ignore_signal_handlers(shell);
// 	if (waitpid(child_pid, &child_status, 0) == -1)
// 		perror("minishell: waitpid");
// 	setup_prompt_signals(shell);
// }
