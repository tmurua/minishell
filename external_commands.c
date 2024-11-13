/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 08:57:29 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/13 20:10:32 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	fork new child process with fork_child_process(), if forking works execute
	external command in child process with execute_in_child(), if not run
	handle_parent_process() so parent process waits for child proces to finish*/
void	execute_external_cmd(char **cmd_args, char **envp, t_token *tokens)
{
	pid_t	child_pid;

	child_pid = fork_child_process();
	if (child_pid < 0)
		return ;
	if (child_pid == 0)
		execute_in_child(cmd_args, envp, tokens);
	else
		handle_parent_process(child_pid);
}

/*	fork new child process with fork() and return PID of child process */
pid_t	fork_child_process(void)
{
	pid_t	child_pid;

	child_pid = fork();
	if (child_pid < 0)
		perror("minishell: fork");
	return (child_pid);
}

/*	reset signal handlers to default in child process
	execute external command in child process with excve() */
void	execute_in_child(char **cmd_and_args, char **envp, t_token *tokens)
{
	reset_signal_handlers();
	execve(cmd_and_args[0], cmd_and_args, envp);
	perror("minishell: execve");
	free_arguments(cmd_and_args);
	free_tokens(tokens);
	exit(EXIT_FAILURE);
}


/*	ignore_signal_handlers to prevent shell interruptions while waiting
	wait for child process to finish, and after is finished
	restore signal handlers with running setup_prompt_signals() */
void	handle_parent_process(pid_t child_pid)
{
	int	child_status;

	ignore_signal_handlers();
	if (waitpid(child_pid, &child_status, 0) == -1)
		perror("minishell: waitpid");
	setup_prompt_signals();
}
