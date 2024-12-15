/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:23:19 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/15 18:38:57 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

#define READ_END 0
#define WRITE_END 1

/* initialize pipe between two commands represented by AST nodes */
void	init_pipe(t_ast_node *node, t_minishell *shell)
{
	int		fds[2];
	pid_t	pids[2];
	int		status_left;
	int		status_right;

	if (pipe(fds) == -1)
	{
		perror("pipe");
		return ;
	}
	shell->cmd_in_execution = 1;
	pids[0] = fork_left_child(fds, node->left, shell);
	pids[1] = fork_right_child(fds, node->right, shell);
	close(fds[READ_END]);
	close(fds[WRITE_END]);
	if (waitpid(pids[0], &status_left, 0) == -1)
		perror("waitpid");
	if (waitpid(pids[1], &status_right, 0) == -1)
		perror("waitpid");
	if (WIFEXITED(status_right))
		shell->last_exit_status = WEXITSTATUS(status_right);
	else if (WIFSIGNALED(status_right))
		shell->last_exit_status = 128 + WTERMSIG(status_right);
	shell->cmd_in_execution = 0;
}

/*fork left child process, setup writing to pipe; return pid of forked process*/
pid_t	fork_left_child(int fds[], t_ast_node *node, t_minishell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		close(fds[READ_END]);
		if (dup2(fds[WRITE_END], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			gc_free_all(shell->gc_head);
			exit(EXIT_FAILURE);
		}
		close(fds[WRITE_END]);
		read_tree(node, shell);
		gc_free_all(shell->gc_head);
		exit(shell->last_exit_status);
	}
	return (pid);
}

/*	fork right child process, setup reading from pipe;
	return pid of the forked child process */
pid_t	fork_right_child(int fds[], t_ast_node *node, t_minishell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		close(fds[WRITE_END]);
		if (dup2(fds[READ_END], STDIN_FILENO) == -1)
		{
			perror("dup2");
			gc_free_all(shell->gc_head);
			exit(EXIT_FAILURE);
		}
		close(fds[READ_END]);
		read_tree(node, shell);
		gc_free_all(shell->gc_head);
		exit(shell->last_exit_status);
	}
	return (pid);
}
