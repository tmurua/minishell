/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:02:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/07 10:21:01 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_whitespace(t_lexer *lexer)
{
	while (lexer->current_char != '\0' && ft_iswhitespace(lexer->current_char))
		move_forward(lexer);
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

/* count nbr of tokens in input str*/
int	count_tokens(const char *str)
{
	int	count;
	int	in_token_flag;

	count = 0;
	in_token_flag = 0;
	while (*str != '\0')
	{
		if (!ft_iswhitespace(*str) && !in_token_flag)
		{
			in_token_flag = 1;
			count++;
		}
		else if (ft_iswhitespace(*str))
		{
			in_token_flag = 0;
		}
		str++;
	}
	return (count);
}

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
