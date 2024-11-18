/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:23:19 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/18 18:44:31 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include "../../include/minishell.h"



// redirection still needs to be implemented
int	init_pipe(t_ast_node *node, char ***envp)
{
	int		fds[2];
	pid_t	pids[2];
	int		status;

	pipe(fds);
	pids[0] = fork();
	if (pids[0] == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		read_tree(node->left, envp);
	}
	pids[1] = fork();
	if (pids[1] == 0)
	{
		close(fds[1]);
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		read_tree(node->right, envp);
	}
	close(fds[0]);
	close(fds[1]);
	waitpid(pids[0], &status, 0);
	waitpid(pids[1], &status, 0);
	return (0);
}
