/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 23:18:42 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/27 00:21:01 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_files *create_heredoc_file(char *delimiter, int is_quoted, t_minishell *shell)
{
    t_files *new_file = gc_calloc(&shell->gc_head, 1, sizeof(t_files));
    if (!new_file)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_file->delim = delimiter;
    new_file->fd = -1;
    new_file->heredoc_quote = is_quoted;
    new_file->next = NULL;
    return new_file;
}

t_list *create_heredoc_list(t_minishell *shell)
{
    t_list *new_list = gc_calloc(&shell->gc_head, 1, sizeof(t_list));
    if (!new_list)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_list->content = NULL;
    new_list->next = NULL;
    return new_list;
}

void add_heredoc_to_list(t_files **head, t_files *new_file)
{
    if (!*head)
        *head = new_file;
    else
    {
        t_files *current = *head;
        while (current->next)
            current = current->next;
        current->next = new_file;
    }
}

void add_list_to_outer_list(t_list **head, t_list *new_list)
{
    if (!*head)
        *head = new_list;
    else
    {
        t_list *current = *head;
        while (current->next)
            current = current->next;
        current->next = new_list;
    }
}

int is_quoted_delimiter(char *delimiter)
{
    int i;

    i = 0;
    if (!delimiter)
        return 0;
    while (delimiter[i])
    {
        if (delimiter[i] == '"' || delimiter[i] == '\'')
            return (1);
        i++;
    }
    return (0);
}

void	heredoc_scan(char *input, t_minishell *shell)
{
	t_list		*outer_list;
    t_list		*current_list;
    t_files     *new_heredoc;
    const char	*command_block;
    char		*delimiter;
    char        *end;
    int			is_quoted;

	outer_list = NULL;
	current_list = NULL;
	command_block = input;
	
	while (*command_block)
	{
		if (ft_strncmp(command_block, "<<", 2) == 0)
		{
            command_block += 2;
            while (ft_iswhitespace(*command_block))
                command_block++;
            delimiter = gc_strdup(&shell->gc_head, command_block);
            if (!delimiter)
            {
		        gc_free_all(shell->gc_head);
		        exit(EXIT_FAILURE);
            }
            end = delimiter;
            while (*end && !ft_iswhitespace(*end) && *end != '|' && *end != '&')
                end++;
            *end = '\0';
            is_quoted = is_quoted_delimiter(delimiter);
            new_heredoc = create_heredoc_file(delimiter, is_quoted, shell);
            if (!current_list)
            {
                current_list = create_heredoc_list(shell);
                add_list_to_outer_list(&outer_list, current_list);
            }
            add_heredoc_to_list((t_files **)&current_list->content, new_heredoc);
        }
        if (*command_block == '|' || (*command_block == '&' && *(command_block + 1) == '&'))
        {
            current_list = NULL;
            if (*command_block == '&')
                command_block++;
        }
		command_block++;
	}
    shell->heredocs = outer_list;
}