/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:41:00 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/11 22:44:13 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*handle_heredoc_token(t_command *cmd, t_token *token,
		t_minishell *shell)
{
	if (token->next && token->next->type == TOKEN_HEREDOC_DELIMITER)
	{
		add_heredoc_to_cmd(cmd, token->next->value, shell);
		return (token->next->next);
	}
	return (token->next);
}

void	add_heredoc_to_cmd(t_command *cmd, char *delimiter, t_minishell *shell)
{
	t_files	*new_infile;
	t_files	*current;

	cmd->has_heredoc = 1;
	new_infile = gc_calloc(&shell->gc_head, 1, sizeof(t_files));
	if (!new_infile)
	{
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	new_infile->delim = gc_strdup(&shell->gc_head, delimiter);
	new_infile->next = NULL;
	new_infile->fd = get_last_heredoc_fd((t_files *)(shell->heredocs->content));
	if (new_infile->fd < 0)
		new_infile->fd = open("/dev/null", O_RDONLY);
	if (!cmd->infile)
		cmd->infile = new_infile;
	else
	{
		current = cmd->infile;
		while (current->next)
			current = current->next;
		current->next = new_infile;
	}
}

int	get_last_heredoc_fd(t_files *heredocs)
{
	t_files	*current;

	if (!heredocs)
		return (-1);
	current = heredocs;
	while (current->next)
		current = current->next;
	return (current->fd);
}
