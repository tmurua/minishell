/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 23:18:42 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/27 00:21:01 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../include/minishell.h"

// int	is_heredoc_delimiter(const char *input, const char *delimiter);

// void	heredoc_loop(t_token *token, int *pipe)
// {
// 	char	*buffer;
// 	char	*delimiter;

// 	buffer = NULL;
// 	delimiter = token->next->value;
// 	close(pipe[0]);
// 	while (1)
// 	{
// 		buffer = readline("> ");
// 		if (!buffer || is_heredoc_delimiter(buffer, delimiter))
// 			break ;
// 		//expension
// 	}
// 	close(pipe[1]);
// 	//close_heredoc
// 	//exit
// }

// void	init_heredoc(t_minishell *shell, t_token *token)
// {
// 	int		fd[2];
// 	pid_t	pid;
// 	int		status;

// 	if (pipe(fd) < 0)
// 		return ;
// 	add_heredoc(shell->heredoc, fd[0], token->next->value);
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		shell->heredocs->fd = fd[1];
// 		heredoc_loop(token, fd);
// 	}
// 	if (waitpid(pid, &status, 0) == -1)
// 		perror("minishell: waitpid");
// 	close(fd[1]);
// }


// int	is_heredoc_delimiter(const char *input, const char *delimiter)
// {
// 	size_t	delimiter_len;

// 	delimiter_len = ft_strlen(delimiter);
// 	if (ft_strncmp(input, delimiter, delimiter_len) == 0)
// 	{
// 		if (input[delimiter_len] == '\0' || input[delimiter_len] == '\n')
// 			return (1);
// 	}
// 	return (0);
// }

// void	add_heredoc(t_minishell *shell, int fd, char *delimiter)
// {
// 	t_files	*new_heredoc;
// 	t_files	*current;

// 	new_heredoc = gc_calloc(&shell->gc_head, 1, sizeof(t_files));
// 	if (!new_heredoc)
// 	{
// 		gc_free_all(shell->gc_head);
// 		exit(EXIT_FAILURE);
// 	}
// 	new_heredoc->delim = delimiter;
// 	new_heredoc->next = NULL;
// 	new_heredoc->fd = fd;
// 	// if (new_heredoc->fd < 0)
// 	// {
// 	// 	perror(filename);
// 	// 	new_heredoc->fd = open("/dev/null", O_WRONLY);
// 	// }
// 	if (!shell->heredocs)
// 		shell->heredocs = new_heredoc;
// 	else
// 	{
// 		current = shell->heredocs;
// 		while (current->next)
// 			current = current->next;
// 		current->next = new_outfile;
// 	}
// }

////////

// void	init_heredoc(t_command *cmd, char *delimiter)
// {
// 	t_files	*new_infile;
// 	t_files	*current;
// 	char	*input;

// 	new_infile = malloc(sizeof(t_files));
// 	if (!new_infile)
// 		exit(EXIT_FAILURE);
// 	new_infile->delim = delimiter;
// 	new_infile->next = NULL;
// 	input = NULL;
// 	while (!is_heredoc_delimiter(input, delimiter))
// 	{
// 		input = readline("> ");
// 	}
// 	if (!cmd->infile)
// 		cmd->infile = new_infile;
// 	else
// 	{
// 		current = cmd->infile;
// 		while (current->next)
// 			current = current->next;
// 		current->next = new_infile;
// 	}
// }
