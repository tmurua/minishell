/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:02:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/08 13:45:41 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	join 2 str and free 1st str to prevent mem leaks, used on advance_and_append
	to expand buffer by appending char to end of buffer str without mem leaks */
char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*join;

	join = ft_strjoin(s1, s2);
	if (s1 != NULL)
		free(s1);
	return (join);
}

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
t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		perror("minishell: create_token");
		exit(EXIT_FAILURE);
	}
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

/* free entire linked list of tokens starting from current token */
void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next_token;

	current = tokens;
	while (current != NULL)
	{
		next_token = current->next;
		free(current->value);
		free(current);
		current = next_token;
	}
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
