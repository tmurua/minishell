/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_parse_args.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 21:12:04 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/13 21:29:33 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**parse_arguments(t_list **gc_head, const char *input)
{
	const char	*ptr;
	char		*arg;
	char		*clean_input;
	t_list		*args_list;

	clean_input = prepare_input(gc_head, input);
	if (!clean_input || *clean_input == '\0')
		return (NULL);
	ptr = clean_input;
	args_list = NULL;
	while (1)
	{
		skip_spaces(&ptr);
		if (*ptr == '\0')
			break ;
		if (*ptr == '"' || *ptr == '\'')
			arg = read_quoted_arg(gc_head, &ptr, *ptr);
		else
			arg = read_unquoted_arg(gc_head, &ptr);
		if (!arg)
			return (NULL);
		add_arg_to_list(gc_head, &args_list, arg);
	}
	return (list_to_array(gc_head, args_list));
}

char	*prepare_input(t_list **gc_head, const char *input)
{
	char	*trimmed;

	if (!input)
		return (NULL);
	trimmed = gc_strtrim(gc_head, input, " \t\n\v\f\r");
	return (trimmed);
}

void	add_arg_to_list(t_list **gc_head, t_list **args_list, char *arg)
{
	t_list	*new_node;
	t_list	*temp;

	new_node = gc_calloc(gc_head, 1, sizeof(t_list));
	if (!new_node)
		return ;
	new_node->content = arg;
	new_node->next = NULL;
	if (!*args_list)
		*args_list = new_node;
	else
	{
		temp = *args_list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
}

char	**list_to_array(t_list **gc_head, t_list *args_list)
{
	int		count;
	t_list	*temp;
	char	**args;
	int		i;

	count = 0;
	temp = args_list;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	args = gc_calloc(gc_head, count + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	while (args_list)
	{
		args[i++] = (char *)args_list->content;
		args_list = args_list->next;
	}
	args[i] = NULL;
	return (args);
}
