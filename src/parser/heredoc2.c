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

char    *get_delimiter(char *input, t_minishell *shell);
int     is_quoted_delimiter(char *delimiter);
t_files *create_heredoc_file(char *delimiter, int is_quoted, t_minishell *shell);
t_list  *create_heredoc_list(t_minishell *shell);
void    add_heredoc_to_list(t_files **head, t_files *new_file);
void    add_list_to_outer_list(t_list **head, t_list *new_list);

void	heredoc_scan(char *input, t_minishell *shell)
{
	t_list		*outer_list;
    t_list		*current_list;
    t_files     *new_heredoc;
    char		*delimiter;
    int			is_quoted;

	outer_list = NULL;
	current_list = NULL;
	
	while (*input)
	{
		if (ft_strncmp(input, "<<", 2) == 0)
		{
            input += 2;
            while (ft_iswhitespace(*input))
                input++;
            delimiter = get_delimiter(input, shell);
            is_quoted = is_quoted_delimiter(delimiter);
            new_heredoc = create_heredoc_file(delimiter, is_quoted, shell);
            if (!current_list)
            {
                current_list = create_heredoc_list(shell);
                add_list_to_outer_list(&outer_list, current_list);
            }
            add_heredoc_to_list((t_files **)&current_list->content, new_heredoc);
        }
        if (*input == '|' || (*input == '&' && *(input + 1) == '&'))
        {
            current_list = NULL;
            if (*input == '&')
                input++;
        }
		input++;
	}
    shell->heredocs = outer_list;
}

char *get_delimiter(char *input, t_minishell *shell)
{
    int     i;
    char    *delimiter;

    delimiter = gc_strdup(&shell->gc_head, input);
    if (!delimiter)
    {
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
    }
    i = 0;
    while (delimiter[i] && !ft_iswhitespace(delimiter[i]) 
                && delimiter[i] != '|' && delimiter[i] != '&')
        i++;
    delimiter[i] = '\0';
    return (delimiter);
}

int is_quoted_delimiter(char *delimiter)
{
    int i;

    i = 0;
    if (!delimiter)
        return (0);
    while (delimiter[i])
    {
        if (delimiter[i] == '"' || delimiter[i] == '\'')
            return (1);
        i++;
    }
    return (0);
}

t_files *create_heredoc_file(char *delimiter, int is_quoted, t_minishell *shell)
{
    t_files *new_file;
    
    new_file = gc_calloc(&shell->gc_head, 1, sizeof(t_files));
    if (!new_file)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_file->delim = delimiter;
    new_file->fd = -1;
    new_file->heredoc_quote = is_quoted;
    new_file->next = NULL;
    return (new_file);
}

t_list *create_heredoc_list(t_minishell *shell)
{
    t_list *new_list;
    
    new_list = gc_calloc(&shell->gc_head, 1, sizeof(t_list));
    if (!new_list)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_list->content = NULL;
    new_list->next = NULL;
    return (new_list);
}

void add_heredoc_to_list(t_files **head, t_files *new_file)
{
    t_files *current;
    
    if (!*head)
        *head = new_file;
    else
    {
        current = *head;
        while (current->next)
            current = current->next;
        current->next = new_file;
    }
}

void add_list_to_outer_list(t_list **head, t_list *new_list)
{
    t_list *current;
    
    if (!*head)
        *head = new_list;
    else
    {
        current = *head;
        while (current->next)
            current = current->next;
        current->next = new_list;
    }
}
