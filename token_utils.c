/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:02:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/03 21:00:47 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
