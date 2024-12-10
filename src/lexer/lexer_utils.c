/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:02:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/10 20:02:59 by dlemaire         ###   ########.fr       */
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

void	update_redirect_tokens(t_token *tokens)
{
	while (tokens)
	{
		if ((tokens->type == TOKEN_REDIRECT_IN
				|| tokens->type == TOKEN_REDIRECT_OUT
				|| tokens->type == TOKEN_REDIRECT_APPEND)
			&& tokens->next)
			tokens->next->type = TOKEN_FILENAME;
		if (tokens->type == TOKEN_HEREDOC)
		{
			if (tokens->next)
				tokens->next->type = TOKEN_HEREDOC_DELIMITER;
			else
				ft_putstr_fd("minishell: syntax error near unexpected token\n",
					2);
		}
		tokens = tokens->next;
	}
}
