/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:02:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/26 19:06:44 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_tokens(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/* creates new token with specified type and value */
t_token	*create_token(t_token_type type, char *value, t_minishell *shell)
{
	t_token	*token;

	token = gc_calloc(&shell->gc_head, 1, sizeof(t_token));
	if (!token)
	{
		perror("minishell: create_token");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

/*	adds a new token to the end of the tokens linked list */
void	token_to_list(t_token **tokens, t_token **current, t_token *new)
{
	if (!*tokens)
		*tokens = new;
	else
		(*current)->next = new;
	*current = new;
}
