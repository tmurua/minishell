/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 16:42:16 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/02 17:17:35 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_next_token(t_lexer *lexer)
{
	t_token	*token;
	char	*value;

	skip_whitespace(lexer);
	if (lexer->current_char == '\0')
		return (NULL);
	if (ft_isalnum(lexer->current_char))
	{
		value = collect_word(lexer);
		if (is_builtin_command(value))
			token = create_token(BUILTIN_CMD, value);
		else
			token = create_token(ARGUMENT, value);
		return (token);
	}
	else
	{
		value = collect_symbol(lexer);
		// add logic to determine type of symbol
		token = create_token(SYMBOL, value);
		return (token);
	}
}

void	skip_whitespace(t_lexer *lexer)
{
	while (lexer->current_char != '\0' && ft_iswhitespace(lexer->current_char))
		move_forward(lexer);
}

char	*collect_word(t_lexer *lexer)
{
	size_t	start_pos;
	char	*word;

	start_pos = lexer->pos;
	while (lexer->current_char != '\0'
		&& ft_isalnum(lexer->current_char))
		move_forward(lexer);
	word = ft_substr(lexer->str, start_pos, lexer->pos - start_pos);
	return (word);
}

char	*collect_symbol(t_lexer *lexer)
{
	char	*symbol;

	symbol = ft_substr(lexer->str, lexer->pos, 1);
	move_forward(lexer);
	return (symbol);
}
