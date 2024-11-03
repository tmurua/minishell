/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:35:59 by dlemaire          #+#    #+#             */
/*   Updated: 2024/11/03 20:56:06 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	if (!tokens)
		return ;
	while (tokens)
	{
		free(tokens->value);
		tokens++;
	}
	free(tokens);
}

t_lexer init_lexer(const char *arg)
{
	t_lexer	lexer;

	lexer.str = arg;
	lexer.pos = 0;
	lexer.current_char = arg[0];
	return (lexer);
}

/* moves lexer forward by one char, update current_char based on new position */
void	move_forward(t_lexer *lexer)
{
	lexer->pos++;
	if (lexer->pos < ft_strlen(lexer->str))
		lexer->current_char = lexer->str[lexer->pos];
	else
		lexer->current_char = '\0';
}

t_token	*run_lexer(char *str)
{
	t_lexer	lexer;
	t_token	*tokens;
	t_token	*current_token;
	t_token	*new_token;

	lexer = init_lexer(str);
	tokens = NULL;
	current_token = NULL;
	while (lexer.current_char != '\0')
	{
		new_token = get_next_token(&lexer);
		if (!new_token)
			break ;
		if (new_token->type == INVALID_TOKEN)
		{
			free_tokens(tokens);
			return (NULL);
		}
		if (!tokens)
			tokens = new_token;
		else
			current_token->next = new_token;
		current_token = new_token;
	}
	return (tokens);
}
