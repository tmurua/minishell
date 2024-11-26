/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 23:18:42 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/26 16:04:13 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void	heredoc_loop(t_token *token, int *pipe)
{
	char	*buffer;
	char	*delimiter;

	buffer = NULL;
	delimiter = token->next->value;
	close(pipe[0]);
	while (1)
	{
		buffer = readline("> ");
		if (!buffer || ft_strncmp(buffer, delimiter, ft_strlen(delimiter) + 1) == 0)
			break ;
		//expension
	}
	ft_putstr_fd(NULL, pipe[1]);
	close(pipe[1]);
	//close_heredoc
	//exit
}

void	init_heredoc(t_minishell *shell, t_token *token)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) < 0)
		return ;
	pid = fork();
	if (pid == 0)
	{
		shell->cmd->infile->fd = fd[1];
		heredoc_loop(token, fd);
	}
	if (waitpid(pid, &status, 0) == -1)
		perror("minishell: waitpid");
	close(fd[1]);
}
